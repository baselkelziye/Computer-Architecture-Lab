
#define SELECT12HOURS 1

#if  SELECT12HOURS == 1  
void add_hour(void){
hours = (hours + 1)%13;
if(hours == 12){
  isPM =~isPM;
}

if(hours == 0){
  hours++;
 }        
}

void adjust_clock_string(void){        
  result_char[0] = hour_char[4];
  result_char[1] = hour_char[5];
  result_char[2] = ':';
  result_char[3] = minutes_char[4];
  result_char[4] = minutes_char[5];
  result_char[5] = ':';
  result_char[6] = seconds_char[4];
  result_char[7] = seconds_char[5];
  result_char[9] = 'M';
  if(isPM){
    result_char[8] = 'P';  
  } else{
   result_char[8] = 'A';
  }
                
}


#else
void add_hour(void){
  hours = (hours + 1)%24;
}


void adjust_clock_string(void){        
  result_char[0] = hour_char[4];
  result_char[1] = hour_char[5];
  result_char[2] = ':';
  result_char[3] = minutes_char[4];
  result_char[4] = minutes_char[5];
  result_char[5] = ':';
  result_char[6] = seconds_char[4];
  result_char[7] = seconds_char[5];
  
      
}

#endif