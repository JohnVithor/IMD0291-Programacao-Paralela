#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>
#include <omp.h>

size_t max_number_of_char = 10;
size_t number_of_types_char = 10;
size_t number_of_results = 5;
size_t number_of_queues = 2;

size_t identifier_counter = 0;

typedef struct Client
{
    size_t*     initial_characteristics;
    size_t**    derived_characteristics;
    size_t      number_of_init_chars;
    size_t      identifier;
    size_t      result;
} Client;

size_t rand_between(size_t l, size_t r) { 
    return (size_t) (l + (rand() % (r - l + 1))); 
} 

Client* createClient() {
    Client* client = malloc(sizeof(Client));
    client->number_of_init_chars = rand_between(0, max_number_of_char);
    client->initial_characteristics = malloc(client->number_of_init_chars*sizeof(size_t));
    for (size_t i = 0; i < client->number_of_init_chars; ++i) {
        client->initial_characteristics[i] = rand_between(1, number_of_types_char);
    }
    if(number_of_queues - 1 >= 1){
        client->derived_characteristics = malloc((number_of_queues-1)*sizeof(size_t*));
        for (size_t i = 0; i < number_of_queues-1; ++i) {
            client->derived_characteristics[i] = malloc((client->number_of_init_chars+i+1)*sizeof(size_t));
        }
    }
    client->identifier = identifier_counter++;
    return client;
}

void destroyClient(Client* client){
    free(client->initial_characteristics);
    for (size_t i = 0; i < number_of_queues-1; ++i) {
        free(client->derived_characteristics[i]);
    }
    free(client->derived_characteristics);
    free(client);
}

void printClient(Client* client, short int detail_level) {
    printf("\nId: %ld\n", client->identifier);
    printf("Result: %ld\n", client->result);
    if(detail_level > 0) {
        printf("Nº of characteristics: %ld\n", client->number_of_init_chars);
        if(client->number_of_init_chars > 0) {
            printf("Characteristics: %ld", client->initial_characteristics[0]);
            for (size_t i = 1; i < client->number_of_init_chars; ++i) {
                printf(", %ld", client->initial_characteristics[i]);
            }
        }
        for (size_t i = 0; i < number_of_queues - 1; ++i) {
            if(detail_level > i+1) {
                if(client->number_of_init_chars > 0) {
                    printf("\nCharacteristics on queue %ld: %ld", i+1, client->derived_characteristics[i][0]);
                    for (size_t j = 1; j < client->number_of_init_chars+i+1; ++j) {
                        printf(", %ld", client->derived_characteristics[i][j]);
                    }
                }
            }
        }
    }
    printf("\n");
}

size_t initialCharProcess(Client* client) {
    size_t value = 0;
    for (size_t i = 0; i < client->number_of_init_chars; ++i) {
        value += client->initial_characteristics[i];
    }
    printf("%ld, ", value);
    return (value / (client->number_of_init_chars + 1)) + (value % (client->number_of_init_chars + 1));
}

size_t levelCharProcess(Client* client, size_t* values, size_t level) {
    size_t value = 0;
    size_t* origin = NULL;
    if (level == 0) {
        origin = client->initial_characteristics;
    } else {
        origin = client->derived_characteristics[level-1];
    }
    client->derived_characteristics[level][0] = origin[0] + values[level];
    for (size_t i = 0; i < (client->number_of_init_chars+level); ++i) {
        client->derived_characteristics[level][i] = abs(origin[i] - origin[i+1]);
        value += client->derived_characteristics[level][i];
    }
    client->derived_characteristics[level][client->number_of_init_chars+level] = abs(origin[client->number_of_init_chars+level] - origin[0]);
    value += client->derived_characteristics[level][client->number_of_init_chars+level];
    return value * client->number_of_init_chars;  
}

size_t categoryFromValue(size_t* values) {
    size_t result = 0;
    for (size_t i = 0; i < number_of_queues; ++i) {
        result += values[i] * (i+1);
    }
    result = result / number_of_queues;

    return result % number_of_results;
}

void categorizeClient(Client* client) {
    size_t* values = calloc(number_of_queues, sizeof(size_t));
    values[0] = initialCharProcess(client);
    printf("%ld", values[0]);
    for (size_t i = 0; i < number_of_queues-1; ++i) {
        values[i] += levelCharProcess(client, values, i);
    }
    
    client->result = categoryFromValue(values);
    free(values);
}

long convert_str_long(char *str){
    char *p;
    errno = 0;
    long conv = strtol(str, &p, 10);

    if (errno != 0 || *p != '\0')
    {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (long)conv;
}

int main(int argc, char **argv){

    if (argc != 7) {
        printf("É necessário informar os seguintes argumentos:\n");
        return -1;
    }

    unsigned int seed = convert_str_long(argv[1]);

    max_number_of_char = convert_str_long(argv[2]);
    number_of_types_char = convert_str_long(argv[3]);
    number_of_results = convert_str_long(argv[4]);
    number_of_queues = convert_str_long(argv[5]);
    size_t detail_level = convert_str_long(argv[6]);

    srand(seed);
    Client* client = createClient();
    categorizeClient(client);
    printClient(client, detail_level);
    destroyClient(client);

    return 0;
} /* main */