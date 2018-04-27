#include <stdio.h>
#include <stdlib.h>
#include <omp.h>   
#include <time.h>
void sort(void) {

  	int my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();

}

int main(int argc, char* argv[]) {
	int start = clock();
	int n = 15000;
	int a[n];
	int thread_count = strtol(argv[1], NULL, 10); 
  	int i,j,count;
	int* temp = malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		a[i] = rand();
	}

   	for(i=0;i<n;i++){
		count = 0;
#  	pragma omp parallel for num_threads(thread_count) schedule(guided,10)
		for(j=0;j<n;j++){
			if(a[i]>a[j]){count++;}
			else if(a[i]==a[j] && j<i){count++;}
		}
		temp[count] = a[i];
	}


	memcpy(a,temp,n*sizeof(int));
	free(temp);
	int end = clock();
	int total = end - start;
	for(i=0;i<n;i=i+5){
		printf("%d %d %d %d %d\n",a[i],a[i+1],a[i+2],a[i+3],a[i+4]);
	}
	printf("total time: %d \n",total);
   	return 0; 
}
