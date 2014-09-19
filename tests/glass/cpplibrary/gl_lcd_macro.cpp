inline bool NOP(){
  Sendbyte_glcd(0,0x00);
  return 0;
}
inline bool SOFT_RESET(){
  Sendbyte_glcd(0,0x01);
  return 0;
}
inline bool ENTER_SLEEP_MODE(){
  Sendbyte_glcd(0,0x10);
  return 0;
}
inline bool EXIT_SLEEP_MODED(){
  Sendbyte_glcd(0,0x11);
  return 0;
}
inline bool ENTER_PARTIAL_MODE(){
  Sendbyte_glcd(0,0x12);
  return 0;
}
inline bool ENTER_NORMAL_MODE(){
  Sendbyte_glcd(0,0x13);
  return 0;
}
inline bool EXIT_INVERT_MODE(){
  Sendbyte_glcd(0,0x20);
  return 0;
}
inline bool ENTER_INVERT_MODE(){
  Sendbyte_glcd(0,0x21);
  return 0;
}
inline bool SET_GAMMA_CURVE(uchar one){
  Sendbyte_glcd(0,0x26);
  Sendbyte_glcd(1,one);
  return 0;
}
inline bool SET_DISPLAY_OFF(){
  Sendbyte_glcd(0,0x28);
  return 0;
}
inline bool SET_DISPLAY_ON(){
  Sendbyte_glcd(0,0x29);
  return 0;
}
inline bool SET_COLUMN_ADDRESS(uchar x_start,uchar x_end){
  Sendbyte_glcd(0,0x2a);
  Sendbyte_glcd(1,0);
  Sendbyte_glcd(1,x_start);
  Sendbyte_glcd(1,0);
  Sendbyte_glcd(1,x_end);
  return 0;
}
inline bool SET_PAGE_ADDRESS(uchar y_start,uchar y_end){
  Sendbyte_glcd(0,0x2b);
  Sendbyte_glcd(1,0);
  Sendbyte_glcd(1,y_start);
  Sendbyte_glcd(1,0);
  Sendbyte_glcd(1,y_end);
  return 0;
}
inline bool WRITE_MEMORY_START(){
  Sendbyte_glcd(0,0x2c);
  return 0;
}
inline bool WRITE_LUT(){
  Sendbyte_glcd(0,0x2d);
  return 0;
}
inline bool SET_PARTIAL_AREA(uchar one,uchar two,uchar three,uchar four){
  Sendbyte_glcd(0,0x30);
  Sendbyte_glcd(1,one);
  Sendbyte_glcd(1,two);
  Sendbyte_glcd(1,three);
  Sendbyte_glcd(1,four);
  return 0;
}
inline bool SET_SCROLL_AREA(uchar one,uchar two,uchar three,uchar four,uchar five,uchar six){
  Sendbyte_glcd(0,0x33);
  Sendbyte_glcd(1,one);
  Sendbyte_glcd(1,two);
  Sendbyte_glcd(1,three);
  Sendbyte_glcd(1,four);
  Sendbyte_glcd(1,five);
  Sendbyte_glcd(1,six);
  return 0;
}
inline bool SET_TEAR_OFF(){
  Sendbyte_glcd(0,0x34);
  return 0;
}
inline bool SET_TEAR_ON(uchar one){
  Sendbyte_glcd(0,0x35);
  Sendbyte_glcd(1,one);
  return 0;
}
inline bool SET_ADDRESS_MODE(uchar one){
  Sendbyte_glcd(0,0x36);
  Sendbyte_glcd(1,one);
  return 0;
}
inline bool SET_SCROLL_START(uchar one,uchar two){
  Sendbyte_glcd(0,0x37);
  Sendbyte_glcd(1,one);
  Sendbyte_glcd(1,two);
  return 0;
}
inline bool EXIT_IDLE_MODE(){
  Sendbyte_glcd(0,0x38);
  return 0;
}
inline bool ENTER_IDLE_MODE(){
  Sendbyte_glcd(0,0x39);
  return 0;
}
inline bool SET_PIXEL_FORMAT(uchar format){
  Sendbyte_glcd(0,0x3a);
  Sendbyte_glcd(1,format);
  return 0;
}

inline bool WRITE_MEMORY_CONTINUE ()
{
  Sendbyte_glcd(0,0x3c);
  return 0;
}
inline bool SET_TEAR_SCANLINE(uchar one,uchar two)
{
  Sendbyte_glcd(0,0x44);
  Sendbyte_glcd(1,one);
  Sendbyte_glcd(1,two);
  return 0;
}
