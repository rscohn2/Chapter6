#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

double do_sum_novec(double *var, long ncells);
double do_sum(double *var, long ncells);
double do_kahan_sum(double *var, long ncells);
double do_serial_sum_v(double *var, long ncells);
double do_serial_sum_v8(double *var, long ncells);
double do_serial_sum_fog_v(double *var, long ncells);
double do_serial_sum_fog_v8(double *var, long ncells);
double do_kahan_sum_v(double *var, long ncells);
double do_kahan_sum_gcc_v(double *var, long ncells);
double do_kahan_sum_agner_v(double *var, long ncells);
double do_kahan_sum_v8(double *var, long ncells);
double do_kahan_sum_gcc_v8(double *var, long ncells);
double do_kahan_sum_agner_v8(double *var, long ncells);

void cpu_timer_start(struct timeval *tstart_cpu);
double cpu_timer_stop(struct timeval tstart_cpu);

#define ORDERS_OF_MAGNITUDE 1.0e9;

typedef unsigned int uint;

int main(int argc, char *argv[])
{
  for (int pow_of_two = 8; pow_of_two < 31; pow_of_two++){
      long ncells = (long)pow((double)2,(double)pow_of_two);
      long ncellsdiv2 = ncells/2;

      printf("SETTINGS INFO -- ncells %ld log %d\n",ncells,(int)log2((double)ncells));

      double high_value = 1.0e-1;
      double low_value  = 1.0e-1/ORDERS_OF_MAGNITUDE;
      double accurate_sum = (double)ncellsdiv2 * high_value +
                            (double)ncellsdiv2 * low_value;

      double *energy = (double *)malloc(ncells*sizeof(double));

      // Initialize with high values first
      printf("Initializing mesh with Leblanc problem, high values first\n");
      for (long i = 0; i < ncells; i++){
         energy[i] = (i < ncellsdiv2) ? high_value : low_value;
      }

      double test_sum;
      struct timeval cpu_timer;
      double cpu_time;

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_sum_novec(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Serial sum\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Kahan sum with double double accumulator\n");

//******************************************************
      printf(" 4 wide vectors serial sum\n");
//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_sum(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Serial sum (OpenMP SIMD pragma vectorization)\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Intel vector intrinsics Serial sum\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_fog_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Fog intrinsic vector Serial sum\n");

//******************************************************
      printf(" 4 wide vectors Kahan sum\n");
//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Vectorized sum with double double kahan sum accumulator\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_gcc_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   GCC vector extensions sum with double double kahan sum accumulator\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_agner_v(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   Fog C++ vector class sum with double double kahan sum accumulator\n");

//******************************************************
      printf(" 8 wide vector serial sum\n");
//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Intel vector intrinsic Serial sum\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_serial_sum_fog_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Fog intrinsic vector Serial sum\n");

//******************************************************
      printf(" 8 wide vector Kahan sum\n");
//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Vectorized sum with double double kahan sum accumulator\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_gcc_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide GCC vector extensions sum with double double kahan sum accumulator\n");

//******************************************************

      cpu_timer_start(&cpu_timer);

      test_sum = do_kahan_sum_agner_v8(energy, ncells);

      cpu_time = cpu_timer_stop(cpu_timer);
   
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf",
             accurate_sum,test_sum,(test_sum-accurate_sum),((test_sum-accurate_sum)/accurate_sum), cpu_time);
      printf("   8 wide Fog C++ vector class sum with double double kahan sum accumulator\n");

//******************************************************
      free(energy);

      printf("\n");
   }
}

void cpu_timer_start(struct timeval *tstart_cpu){
   gettimeofday(tstart_cpu, NULL);
}

double cpu_timer_stop(struct timeval tstart_cpu){
   double result;
   struct timeval tstop_cpu, tresult;

   gettimeofday(&tstop_cpu, NULL);
   tresult.tv_sec = tstop_cpu.tv_sec - tstart_cpu.tv_sec;
   tresult.tv_usec = tstop_cpu.tv_usec - tstart_cpu.tv_usec;
   result = (double)tresult.tv_sec + (double)tresult.tv_usec*1.0e-6;
   return(result);
}
