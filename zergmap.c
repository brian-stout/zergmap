#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "binary.h"
#include "list.h"
#include "graph.h"
#include "tree.h"
#include "input.h"



int
main( int argc, char *argv[] )
{
    //SUPPORT MULTIPLE FILES AND OPTIONS
    if (argc < 2)
    {
        printf("ERROR, USAGE: [zergmap] [~/<filename>.pcap]\n");
        return EX_USAGE;
    }

    tree * unitTree = NULL;

    for(int i = 1; i < argc; ++i)
    {
        FILE *fp = fopen(argv[i], "rb");

        if (!fp)
        {
            printf("Unable to open file!\n");
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
    //TODO: If there's just two members, report as fully connected

    list * unitList = init_list();
    unitList = unpack_tree(unitTree, unitList);

    struct graph * myGraph;
    myGraph = create_graph(unitList);
    myGraph = cleanup_graph(myGraph);

    print_matrix_table(myGraph);

    if(!graph_solveable(myGraph))
    {
        printf("There is no solution for this problem set!\n");
    }

    //TODO: PROBLEMS:
    //2. This function does not handle nodes to close properly (just gets rid of them)
    //4. No destroy up yet.
    //5. Program does not detect problem packets IE.  Packets whose lengths are too short to contain useful information and would cause the program to read over into another packet
}


