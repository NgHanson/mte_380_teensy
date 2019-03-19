#include "MathHelper.h"
#include "Constants.h"

float calculateAdjustedMean(float data[], float mean, float stdDev) {
  float sum = 0.0, count = 0.0;
  for (int i = 0; i < SENSOR_NUM_SAMPLES; ++i) {
    if (data[i] >= mean - stdDev && data[i] <= mean + stdDev) {
      sum += data[i];
      count += 1.0;
    }
  }
  return sum/count;
}
float calculateMean(float data[]) {
    float sum = 0.0;
    for(int i = 0; i < SENSOR_NUM_SAMPLES; ++i) {
        sum += data[i];
    }
    return sum/SENSOR_NUM_SAMPLES;
}
float calculateSD(float data[], float mean) {
    float standardDeviation = 0.0;
    for(int i = 0; i < SENSOR_NUM_SAMPLES; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    return sqrt(standardDeviation/SENSOR_NUM_SAMPLES);
}
float filteredMean(float data[]){
  float mean = calculateMean(data);
  float std_dev = calculateSD(data, mean);
  return calculateAdjustedMean(data, mean, std_dev);
}
