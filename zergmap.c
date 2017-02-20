#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#include "binary.h"
#include "list.h"
#include "graph.h"
#include "tree.h"
#include "input.h"
#include "output.h"

bool
argument_checker(int argc, char *argv[])
{
    bool r = false;

    for (int i = 0; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            //Checks to see if a tack option is followed by a space
            if (argv[i][1] == ' ' || argv[i][1] == '\0')
            {
                printf("Usage: [-h <zerg health percent>] *[files]\n");
                r = true;
            }
            //Checks to see if non option arguments exist before the options
            else if (argv[1][0] != '-')
            {
                printf("Usage: [-h <zerg health percent>] *[files]\n");
                r = true;
            }
        }
    }

    return r;
}

int
main( int argc, char *argv[] )
{
    if (argument_checker(argc, argv))
    {
        return EX_USAGE;
    }


    //SUPPORT MULTIPLE FILES AND OPTIONS
    if (argc < 2)
    {
        printf("ERROR, USAGE: [zergmap] [~/<filename>.pcap]\n");
        return EX_USAGE;
    }

    int zergHealthPercentage = 10;

    int c;

    while (-1 < (c = getopt(argc, argv, "h:")))
    {
        char *err;

        switch (c)
        {
        case 'h':
            //Grabs the string following c and turns it to a number
            zergHealthPercentage = strtol(optarg, &err, 10);
            //if strtol fails it's likely because of this error
            if (*err)
            {
                printf("Error: -h must be followed by a valid number \n");
                return EX_USAGE;
            }
            //avoids weird negative numbers as user input
            if (zergHealthPercentage < 0)
            {
                printf
                    ("Error: -h can not be followed by a negative number \n");
                return EX_USAGE;
            }
            if (zergHealthPercentage > 100)
            {
                printf
                    ("Error: -h can not be followed by a number greater than 100 \n");
                return EX_USAGE;
            }
            break;
        case '?':
            return EX_USAGE;
        default:
            break;
        }
    }

    tree * unitTree = NULL;

    //Using optind to start after the options have been parsed
    for(int i = optind; i < argc; ++i)
    {
        FILE *fp = fopen(argv[i], "rb");

        if (!fp)
        {
            printf("Unable to open file %s!\n", argv[i]);
            //Maybe put a continue here?
            return EX_USAGE;
        }

        //Skip the global header
        fseek(fp, GLOBAL_HEADER_SZ, SEEK_SET);

        while(1)
        {
            uint32_t unixEpoch;
            int bytesRead;
            bytesRead = fread(&unixEpoch, sizeof(unixEpoch), 1, fp);
            if(bytesRead == 0)
            {
                break;
            }

            //Generate unit data
            zerg * zerg_unit = read_pcap_packet(fp);
            
            //Checking to see if zerg_unit was successful
            if(zerg_unit)
            {
                //Do stuff with unit data
                unitTree = insert(unitTree, zerg_unit);
            }
        }
        fclose(fp);


    }
    //Generating a list of zerg members that only contain GPS information so the graph
    //  function doesn't break;
    list * unitList = init_list();
    unitList = unpack_tree(unitTree, unitList);

    struct graph * unitGraph;
    unitGraph = create_graph(unitList);

    //Function removes nodes with only 1 adjacency
    unitGraph = cleanup_graph(unitGraph);

    int weakNode = 0;

    //Checking for choke points, or points with multiple adjacencies where there's no alternate
    //  route around it
    while(true)
    {
        weakNode = check_for_weakness(unitGraph);
        if(weakNode == -1)
        {
            break;
        }
        else
        {
            //We run cleanup_graph again just in case cleaning up created
            //One adjaceny nodes
            unitGraph = solve_weakness(unitGraph, weakNode);
            unitGraph = cleanup_graph(unitGraph);
        }
    }

    //If more than half the zerg units have been deleted the solution isn't solveable
    if(!graph_solveable(unitGraph))
    {
        printf("Too many changes are need for solution to be cost effective!\n");
    }
    else
    {
        printf("\n");
        print_zerg_removal(unitGraph);
        printf("\n");
    }
    

    bool printOnce = false;
    printOnce = print_status(unitTree, zergHealthPercentage, printOnce);
    if(!printOnce)
    {
        printf("No status info for zergs!\n");
    }
    printf("\n");

    destroy_graph(unitGraph);
    destroy_list(unitList);
    destroy_tree(unitTree);
}


