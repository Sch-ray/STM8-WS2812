#include "Adc.h"

u16 adc_get(void){
  return(ADC1_GetConversionValue());
}