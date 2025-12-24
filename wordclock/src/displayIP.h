#ifndef WORDCLOCK_DISPLAY_IP_H  
#define WORDCLOCK_DISPLAY_IP_H  
  
class DisplayIP {  
  public:  
    static void show(int octet1, int octet2);  
    static void clear();  // Add this line  
};  
  
#endif
