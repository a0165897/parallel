#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h> 

int thread_count;

double f(double x){
	double y = x*x;
	return y;
}

double Trap(double left_endpt,double right_endpt,int trap_count,double base_len){
	double estimate,x;
	int i;
	estimate = (f(left_endpt)+f(right_endpt))/2.0;
	for(i=1;i<=trap_count-1;i++){
		x = left_endpt+ i * base_len;
		estimate += f(x);
	}
	estimate = estimate*base_len;
	return estimate;
	/*trap*/
}

void* parallel(void *rank){
	//a=0.0;b=3.0;n=1024;	
	Get_input(my_rank,comm_sz,&a,&b,&n);

	h=(b-a)/n;
	local_n=n/comm_sz;
	lastCoreRst = n%comm_sz;
	if(my_rank!=comm_sz-1){
		local_a=a+my_rank*local_n*h;
		local_b=local_a+local_n*h;
		local_int=Trap(local_a,local_b,local_n,h);
		printf("my_rank:%d local_n:%d local_int:%f\n",my_rank,local_n,local_int);
	}
	else{
		local_a=a+my_rank*local_n*h;
		local_n+=lastCoreRst;
		local_b=local_a+local_n*h;
		local_int=Trap(local_a,local_b,local_n,h);
		printf("my_rank:%d local_n:%d local_int:%f\n",my_rank,local_n,local_int);
	}

	MPI_Allreduce(&local_int,&total_int,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
	
	if(my_rank==0){
		printf("with n = %d trapezoids, estimate\n",n);
		printf("of the integral form %f to %f = %.15e\n",a,b,total_int);
	}
	else{
		printf("with n = %d trapezoids, estimate\n",n);
		printf("of the integral form %f to %f = %.15e\n",a,b,total_int);
	}
}


int main(int argc, char* argv[]){
	long thread;
	pthread_t *thread_handles;	
	thread_count = strtol(argv[1],NULL,10);	
	thread_handles = malloc (thread_count * sizeof(pthread_t));

        for(thread = 0; thread < thread_count ; thread++){
		pthread_create(&thread_handles[thread],NULL,parallel,(void * )thread);
	}
	for(thread = 0 ; thread < thread_count ; thread++){
		pthread_join(thread_handles[thread],NULL);
	}
	
	free(thread_handles);
	return 0;
}





