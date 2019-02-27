#define US_ARR_LEN 10

void setup(void) 
{
  Serial.begin(9600);
}
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
    return sum/10;
}
float calculateSD(float data[], float mean) {
    float standardDeviation = 0.0;
    for(int i = 0; i < US_ARR_LEN; ++i)
        standardDeviation += pow(data[i] - mean, 2);
    return sqrt(standardDeviation/10);
}

void loop(void) {
  float data[] = {0, 4, 4.5, 4.7, 5, 8, 9, 15, 0, 0};  
  float mean = calculateMean(data);
  float stdDev = calculateSD(data, mean);
  float adjMean = calculateAdjustedMean(data, mean, stdDev);
  Serial.print("Mean: ");
  Serial.println(mean);
  Serial.print("Standard Deviation: ");
  Serial.println(stdDev);
  Serial.print("Adjusted Mean: ");
  Serial.println(adjMean);
  delay(1000);
}
