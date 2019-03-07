#include "MathHelper.h"
#define US_ARR_LEN 10

float calculateAdjustedMean(float data[], float mean, float stdDev) {
  float sum = 0.0, count = 0.0;
  for (int i = 0; i < US_ARR_LEN; ++i) {
    if (data[i] >= mean - stdDev && data[i] <= mean + stdDev) {
      sum += data[i];
      count += 1.0;
    }
  }
  return sum/count;
}
float calculateMean(float data[]) {
    float sum = 0.0;
    for(int i = 0; i < US_ARR_LEN; ++i) {
        sum += data[i];
    }
    return sum/US_ARR_LEN;
}
float calculateSD(float data[], float mean) {
    float standardDeviation = 0.0;
    for(int i = 0; i < US_ARR_LEN; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    return sqrt(standardDeviation/US_ARR_LEN);
}
float filteredMean(float data[]){
  float mean = calculateMean(data);
  float std_dev = calculateSD(data, mean);
  return calculateAdjustedMean(data, mean, std_dev);
}
