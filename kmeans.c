#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void printCentroids(int d, int k, const double* centroids)
{
    int i;
    int j;
    for(i=0; i < k; i++)
    {

        for(j=0; j < d; j++)
        {
            if(j < d - 1)
            {
                printf("%f,", *((centroids + i*d) + j));
            }
            else
            {
                printf("%f", *((centroids + i*d) + j));
            }

        }

        printf("\n");

    }
}

int getClosestClusterIndex(int k, int dimensions, const double *centroids, const double *vector)
{
    int index = 0;
    double shortestDistance;

    /*iterate through all observations(vectors)

    //for each vector iterate through all centroids
    */
    int c;
    int d;
    for(c=0; c < k; c++)
    {

        double distance = 0;
        /*for each centroid calculate the distance and save shortest distance*/
        for(d=0; d < dimensions; d++)
        {
            distance += (*((vector + d)) - (*((centroids + c*dimensions) + d))) * (*((vector + d))- *((centroids + c*dimensions) + d));
        }

        if(c == 0 || distance < shortestDistance)
        {
            index = c;
            shortestDistance = distance;
        }


    }
    /*assign the vector j the index of the closest centroid*/




    return index;
}


int main(int argc, char* argv[])
{

    /*
    LOOP PLACERHOLDERS

    */

    int i;
    int j;
    int d;
    int k;
    int p;

    int centroidChanged = 0;




    /*information about the arguments*/
    int clustersNum = atoi(argv[1]);
    int observationsNum = atoi(argv[2]);
    int dimensions = atoi(argv[3]);
    const int MAX_ITER = atoi(argv[4]);




    int amountOfVectors;

    /*vector index array for closest Cluster*/
    int *vectorClusterIndices;
    double *sumOfVectors;

    /*placeholder variables to store temporary values*/
    char c;
    double cord;

    /*create an array of arrays. Each sub-array contains a D-ARY vector*/
    double **vectors;
    double *vectorPointers;

    double **centroids;
    double *centroidPointers;



    assert(argc >= 5);
    assert(dimensions > 0);
    assert(clustersNum > 0);
    assert(observationsNum > 0);
    assert(observationsNum >= clustersNum);


    vectors = calloc(observationsNum, sizeof(double *));
    vectorPointers = calloc(observationsNum*dimensions, sizeof(double));
    assert(vectors != NULL);
    assert(vectorPointers != NULL);

    for(i=0; i<observationsNum; i++)
    {
        vectors[i] = vectorPointers + i*dimensions;
    }




    centroids = calloc(clustersNum, sizeof(double *));
    centroidPointers = calloc(clustersNum*dimensions, sizeof(double));
    assert(centroids != NULL);
    assert(centroidPointers != NULL);

    for(i=0; i<clustersNum; i++)
    {
     centroids[i] = centroidPointers + i*dimensions;
    }




    i=0;
    j=0;
    /*create the array of observations based on the input we received*/
    while(scanf("%lf%c", &cord, &c) == 2)
    {

        /*first K vector values are also centroids*/
        if(i < clustersNum)
        {
            centroids[i][j] = cord;
        }
        vectors[i][j] = cord;

        j++;
        if(j == dimensions)
        {
            i++;
            j = 0;
        }

    }




    /*clustering*/
    for(i=0; i < MAX_ITER; i++)
    {

        vectorClusterIndices = (int *)calloc(observationsNum, sizeof(int));
        assert(vectorClusterIndices != NULL);


        /*iterate through each observation for each centroid*/
        for(j=0; j < observationsNum; j++)
        {
            vectorClusterIndices[j] = getClosestClusterIndex(clustersNum, dimensions, *centroids, vectors[j]);
        }


        /*update all centroids one-by-one*/
        for(k=0; k < clustersNum; k++)
        {

            sumOfVectors = calloc(dimensions, sizeof(double));
            assert(sumOfVectors != NULL);

            /*initialize all vectors to 0*/
            for(d=0; d < dimensions; d++)
            {
                sumOfVectors[d] = 0;

            }

            amountOfVectors = 0;
            for(p=0; p < observationsNum; p++)
            {
                /*if the cluster index associated with current vector is the index of the current cluster we proceed*/
                if(vectorClusterIndices[p] == k)
                {
                    amountOfVectors += 1;
                    for(d=0; d<dimensions; d++)
                    {


                        sumOfVectors[d] += vectors[p][d];


                    }
                }
            }
            for(d=0; d<dimensions; d++)
            {
                /*check if centroid value changed to know if we can continue*/
                if(centroids[k][d] != sumOfVectors[d] / amountOfVectors)
                {
                    centroidChanged = 1;
                }

                if(amountOfVectors == 0)
                {

                    centroids[k][d] = centroids[k][d];
                }
                else
                {
                    centroids[k][d] = sumOfVectors[d] / amountOfVectors;
                }

            }
            free(sumOfVectors);
        }
        free(vectorClusterIndices);

        /*if none of the centroids changed we exit the loop*/
        if(centroidChanged)
        {
            centroidChanged = 0;
        }
        else {
            break;
        }
    }

    printCentroids(dimensions, clustersNum, *centroids);
    free(centroids);
    free(vectors);
    free(vectorPointers);
    free(centroidPointers);



    return 1;
}


