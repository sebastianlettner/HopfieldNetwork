//
// Created by Sebastian Lettner on 06.01.18.
//

#ifndef CI_HOPFIELDNEWTORK_H
#define CI_HOPFIELDNEWTORK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define MAX_INPUT 10
#define IMAGE_X 20
#define IMAGE_Y 10
#define INPUT_SIZE IMAGE_X*IMAGE_Y




/*
 * In the code we follow the convention that '*' are +1 ones and '.' are -1 ones.
 *
 *
 */

typedef struct Input {

    /*
     * Structure for storing the input.
     * train_patterns: Matrix containing the training patterns.
     *                 Images are enrolled into a vector
     *                 The dimensions of the image are saved in image_x and image_y
     * test_patterns: Matrix containing the distorted patterns for testing
     * image_x,image_y(int): dimensions of image
     * num_patterns(int): Number of patterns to store
     * num_test(int): number of test patterns
     */

    int **training_pattern;
    int **test_pattern;

    int image_x;
    int image_y;

    int num_patterns;
    int num_tests;

} input;

typedef struct HopfieldNetwork {

    /*
     * Structure representing the network.
     * It holds the input we received for convenient usage
     * Weights(int**): is an integer matrix containing the connection weights of the neurons. Size: INPUT_SIZE x INPUT_SIZE
     * Threshold(int*): The threshold vector holds the thresholds for each neuron. It is used in the update rule
     * Output:(int**): Output of the network. Restored pictures.
     * Num_neurons(int): Number of neurons in the network. Is equal to INPUT_SIZE
     * Neuron_state(int*): State of every neuron (-1,1). Size: INPUT_SIZE
     */

    struct Input* input;
    int** weights;
    int* threshold;
    int** output;
    int num_neurons;
    int* neurons_state;



} HopfieldNetwork;

void print_array_int(int** a, int y, int x) {

    for (int i = 0; i < y; ++i) {
        for (int j = 0; j < x; ++j) {
            printf("%d, ", a[i][j]);
        }
        printf("\n");
    }
}

void initialize_input(struct Input *input, int num_patterns, int num_tests) {

    /*
     * Function for initializing the Input struct.
     * It allocates the necessary memory.
     * Args:
     *  input(struct Input): Input
     *  num_patterns(int): Number of patterns. Necessary to allocated the correct space
     *  num_tests(int): Number of test patterns. Necessary here to allocate correct space
     *
     * Note: The function only allocates the space for the patterns but does not fill in the values!
     */

    /* Allocating space for patterns to store */
    input->training_pattern = (int**)malloc(sizeof(int*)*num_patterns);
    for (int i = 0; i < num_patterns; ++i) {
        input->training_pattern[i] = (int*)malloc(sizeof(int)*INPUT_SIZE);
    }

    /* Allocating space for the distorted test patterns */
    input->test_pattern = (int**)malloc(sizeof(int*)*num_tests);
    for (int j = 0; j < num_tests; ++j) {
        input->test_pattern[j] = (int*)malloc(sizeof(int)*INPUT_SIZE);
    }

    input->image_x = IMAGE_X;
    input->image_y = IMAGE_Y;
    input->num_patterns = num_patterns;
    input->num_tests = num_tests;

}

void print_picture(int const* picture_as_vector) {

    /*
     * Function prints an image ernolled into a vector as an image in the correct dimensions
     */


    for (int j = 0; j < IMAGE_Y; ++j) {
        for (int i = 0; i < IMAGE_X; ++i) {

            printf("%c", ((picture_as_vector[(j*IMAGE_X)+i] == 1) ? '*' : '.'));
        }
        printf("\n");
    }

}

void fill_input(struct Input* input) {

    char inputLine[IMAGE_X];
    int counter = 0;
    int m = 0;
    int k = 0;

    int num_pattern = 0;
    int num_test = 0;


    int buffer_train[MAX_INPUT][INPUT_SIZE];
    int buffer_test[MAX_INPUT][INPUT_SIZE];


    while(scanf("%s\n", inputLine) != EOF) {
        counter = 0; // reset counter

        for (int i = 0; i < IMAGE_X; ++i) {
            //detect end of each training example or end of training data
            if (inputLine[i] == '-') {
                counter += 1;
            }
            //only save input line in matrix if counter is 0
            if (counter == 0){
                int temp = ((inputLine[i]=='*') ? 1 : -1 );
                buffer_train[k][i+(m*IMAGE_X)] = temp;
            }
        }
        m++;
        if (counter == 1) {
            k++; //k is iterator of num_training_data
            m = 0;
        }

        if (counter == 3) {
            num_pattern = k+1;
            break;
        }

    }

    k = 0;
    m = 0;
    while(scanf("%s\n", inputLine) != EOF) {
        counter = 0; //reset counter

        for (int i = 0; i < IMAGE_X; ++i) {

            //detect end of each test example
            if (inputLine[i] == '-') {
                counter += 1;
            }

            //only save input line in matrix if counter is 0
            if (counter == 0){
                for (int j = 0; j < IMAGE_X; ++j) {
                    int temp = ((inputLine[j]=='*') ? 1 : -1 );
                    buffer_test[k][j+(m*IMAGE_X)] = temp;
                }
            }
        }
        m +=1; //if row of matrix is completed start next row (X-1 columns per row)

        if (counter == 1) {
            k += 1; //k is iterator of num_test_data
            m = 0; // m is iterator of rows
        }
    }
    num_test = k+1;
    initialize_input(input,num_pattern,num_test);
    //Fill in the buffer values
    for (int l = 0; l < num_pattern; ++l) {
        for (int i = 0; i < INPUT_SIZE; ++i) {
            input->training_pattern[l][i] = buffer_train[l][i];
        }
    }
    for (int l = 0; l < num_test; ++l) {
        for (int i = 0; i < INPUT_SIZE; ++i) {
            input->test_pattern[l][i] = buffer_test[l][i];
        }
    }
}

void initialize_network(struct HopfieldNetwork* net, struct Input* input) {

    //Set input
    net->input = input;

    //Init weights with 0
    net->weights = (int**)calloc(INPUT_SIZE, sizeof(int*));
    for (int i = 0; i < INPUT_SIZE ; ++i) {
        net->weights[i] = (int*)calloc(INPUT_SIZE, sizeof(int));
    }

    //Init threshold and network_state
    for (int j = 0; j < INPUT_SIZE; ++j) {
        net->threshold = (int*)calloc(INPUT_SIZE, sizeof(int));
        net->neurons_state = (int*)calloc(INPUT_SIZE, sizeof(int));
    }

    net->num_neurons = INPUT_SIZE;

    //Init output with 0
    net->output = (int**)calloc((size_t)input->num_tests, sizeof(int*));
    for (int k = 0; k < input->num_tests; ++k) {
        net->output[k] = (int*)calloc(INPUT_SIZE, sizeof(int));
    }

}

void calculate_weight_matrix(struct HopfieldNetwork* net) {

    /*
     * This function calculates the weight matrix for the hopfield network
     * We calculate the weight matrix for every example and then add the results together
     *
     * For one input pattern the matrix can be calculated as follows:
     *
     *      w_ij = pattern[i]*pattern[j]   if: i!=j
     *      w_ij = 0    if: i=j
     *
     */
    //for loop over all patterns

    for (int k = 0; k < net->input->num_patterns ; ++k) {

        for (int i = 0; i < INPUT_SIZE; ++i) {

            for (int j = 0; j < INPUT_SIZE; ++j) {
                if (i==j) {
                    net->weights[i][j] = 0;
                }
                else {
                    net->weights[i][j] += net->input->training_pattern[k][i] * net->input->training_pattern[k][j];
                }
            }
        }
    }

}

void set_network_state(struct HopfieldNetwork* net, int test_example) {

    /*
     * This function sets the network state vector to a test pattern indicated with the test_example vaiable.
     * If you want for example restore the second test_pattern you would call this function with
     * your network and test_example = 1 (always -1 because we insert this directly in the array index).
     */
    net->neurons_state = net->input->test_pattern[test_example];

}

int calculate_update_step(struct HopfieldNetwork* net, int neuron_to_update) {

    int  j;
    int  sum, out;
    int changed;

    changed = 0;
    sum = 0;
    for (j=0; j<INPUT_SIZE; j++) {
        sum += net->weights[neuron_to_update][j] * net->neurons_state[j]; //network->output was previously set to input, hence this is h_i=sum(weights*inputs)
    }
    if (sum != net->threshold[neuron_to_update]) {
        if (sum < net->threshold[neuron_to_update]) out = -1;
        if (sum > net->threshold[neuron_to_update]) out = 1;
        if (out != net->neurons_state[neuron_to_update]) {
            changed = 1;
            net->neurons_state[neuron_to_update] = out;
        }
    }
    return changed;

}

void asynchronous_update(struct HopfieldNetwork* net) {

    /*
     * This function performs the asynchronous update meaning we update one neuron randomly
     * The update rule for hopfield networks is as follows:
     *
     *      Assume we want to update neuron i:
     *      Take the ith row of the weight matirx W and perform a dot product with the network_state vector.
     *      Compare this sum to your threshold for neuron i and update as follows:
     *          sum < threshold: neuron_i -> -1
     *          sum > threshold: neuron_i -> +1
     */

    int iteration = 0;
    int iteration_last_change = 0;

    do {

        iteration++;
        if(calculate_update_step(net, rand() % (INPUT_SIZE))) {
            iteration = iteration_last_change;
        }

    } while(iteration-iteration_last_change < 10*INPUT_SIZE);
}

void set_output(struct HopfieldNetwork* net, int test_example) {

    /*
     * Function sets the output(of given test_example) to the current network state
     * Its just done for saving the correction.
     */
    net->output[test_example] = net->neurons_state;
}

void restore_picture(struct HopfieldNetwork* net) {

    /*
     * This function will restore the distorted patterns
     * It goes through the following steps for every test pattern:
     *
     *      1. Set the network state to the pattern we wish to restore
     *      2. Run the update rule to update the neurons until convergence (we perform asynchronous updates)
     *      3. Set the output to the resulting network state
     *
     */
    //for loop over test patterns
    for (int i = 0; i < net->input->num_tests ; ++i) {

        //First step
        set_network_state(net,i);

        //Second step
        asynchronous_update(net);

        //Third step
        set_output(net,i);

    }

}

#endif //CI_HOPFIELDNEWTORK_H
