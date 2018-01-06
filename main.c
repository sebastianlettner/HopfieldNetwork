#include "HopfieldNewtork.h"


int pattern1[9] = {-1,-1,-1,-1,1,-1,-1,-1,-1};
/*
 *    . . .
 *    . * .
 *    . . .
 */
int pattern2[9] = { 1,-1, 1,-1,-1,-1, 1,-1, 1};
/*
 *    * . *
 *    . . .
 *    * . *
 */
int pattern3[9] = {-1,-1, 1,-1, 1,-1, 1,-1,-1};
/*
 *   . . *
 *   . * .
 *   * . .
 */

int test[9] = {-1, 1, -1, -1, 1,-1,-1,-1,-1};
/*
 *   . * .
 *   . * .
 *   . . .
 */


int main() {


    srand((unsigned int) time(NULL));

    /*
     * For testing I will create 3x3 images

    struct Input input_test;
    initialize_input(&input_test,1,1);
    input_test.training_pattern[0] = pattern1;
    input_test.test_pattern[0] = test;
    */
    struct Input input;
    fill_input(&input);
    struct HopfieldNetwork net;
    initialize_network(&net,&input);
    calculate_weight_matrix(&net);
    restore_picture(&net);



    for (int i = 0; i < input.num_tests; ++i) {
        print_picture(net.output[i]);
        if(i<input.num_tests-1){
            printf("-\n");
        }
    }


    return 0;


}
