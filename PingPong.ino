/* clock pins and latch pins vertical and horizotal shift registers used for dotmatrix control*/ 
#define clockpinV 2    
#define clockpinH 3    
#define latchpinH 4    
#define datapinH  6    
#define datapinV  7    
#define latchpinV 8    

uint8_t i;
int t;
int which = 0;

/* Position details of  wedge and ball */
byte position;
byte ballposition;
byte oldpositiontop;
byte newpositiontop;
byte oldpositionbot;
byte newpositionbot;
uint8_t ball;
uint8_t wedge;
int speed=60; // controls the speed of ball
int intial=100;

/* clears the display of dot-matrix */
void cleardisplay()
 {
     digitalWrite(latchpinH,LOW);
     shiftOut(datapinH,clockpinH,LSBFIRST,0b00000000);
     digitalWrite(latchpinH,HIGH);
 }
 
 
/* Reads data from analog POT's and measures position of wedges and return the leds data for the rows containing wedges */
byte  analogit()
{   
    
    int getin = analogRead( which );
    digitalWrite(latchpinV,LOW);
     
      if( which == 0)
      shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<0));
      else
      shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<7));
     
      digitalWrite(latchpinV,HIGH);
   if(which==1)
    which=0;
   else
  which=1; 
   
 if(getin < 172)
   return 0b11100000;
 if(getin < 342)
   return 0b01110000;
 if(getin < 512)
   return 0b00111000;
 if(getin < 682)
   return 0b00011100;
 if(getin < 852)
   return 0b00001110;
 if(getin <= 1024)
   return 0b00000111;

}

/* ending display message i have done it in most dumb way possible because i wrote it way back when i was a shitty coder :P */
void end()
{
  
    for(i=1;i<7;i++)
    {
     	    digitalWrite(latchpinV,LOW);
     	    shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<i));
     	    digitalWrite(latchpinV,HIGH);   
    if(i==1)
    {
      	    digitalWrite(latchpinH,LOW);
      	    shiftOut(datapinH,clockpinH,LSBFIRST,0b00000000);
      	    digitalWrite(latchpinH,HIGH);
    }

    if(i==2)
    {
       	    digitalWrite(latchpinH,LOW);
       	    shiftOut(datapinH,clockpinH,LSBFIRST,0b00100100);
     	    digitalWrite(latchpinH,HIGH);

    }
    if(i==3)
    {
     	     digitalWrite(latchpinH,LOW);
             shiftOut(datapinH,clockpinH,LSBFIRST,0b00000000);
    	     digitalWrite(latchpinH,HIGH);

    }
    if(i==4)
    {
             digitalWrite(latchpinH,LOW);
             shiftOut(datapinH,clockpinH,LSBFIRST,0b00011000);
             digitalWrite(latchpinH,HIGH);

    }
    if(i==5)
    {
             digitalWrite(latchpinH,LOW);
             shiftOut(datapinH,clockpinH,LSBFIRST,0b01000010);
             digitalWrite(latchpinH,HIGH);

    }
    if(i==6)
    {
     	     digitalWrite(latchpinH,LOW);
             shiftOut(datapinH,clockpinH,LSBFIRST,0b00111100);
	     digitalWrite(latchpinH,HIGH);

    }

	   delay(2); 
    }
}


/* intial setup for the game */

void setup()
{ 
  pinMode(clockpinH,OUTPUT);
  pinMode(clockpinV,OUTPUT);
  pinMode(latchpinH,OUTPUT);
  pinMode(datapinH,OUTPUT);
  pinMode(latchpinV,OUTPUT);
  pinMode(datapinV,OUTPUT);
  digitalWrite(latchpinH,HIGH);
  digitalWrite(latchpinV,HIGH);
 for(i=3;i<7;i++)
 {
    for( t=0 ; t<intial; t++) 
    {
   	  cleardisplay();
          digitalWrite(latchpinH,LOW);
  	  position = analogit(); //get position of the wedge
     	if((i==6) && (which == 0))
     	  oldpositionbot = position;

          shiftOut(datapinH,clockpinH,LSBFIRST,position);
          digitalWrite(latchpinH,HIGH);
     
          delay(2);

          cleardisplay();
    
          digitalWrite(latchpinV,LOW);
          shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<i));
          digitalWrite(latchpinV,HIGH);   
 
          digitalWrite(latchpinH,LOW);
          shiftOut(datapinH,clockpinH,LSBFIRST,0b00010000);
          digitalWrite(latchpinH,HIGH);
 
          delay(2);
   }
 }
 ballposition=0b00010000;
 ball=2;
}
   


/*This goes in a loop */
void loop()
{
  
  for(i=6;i>0;i--)
 {
    for( t=0 ; t<speed; t++) 
    {
     cleardisplay();
     
     digitalWrite(latchpinH,LOW);
     position = analogit();
     
     if((i==6) && (which == 0))
     newpositionbot=position;
     
     if((i==1) && (which == 1))
     oldpositiontop=position;
     
     shiftOut(datapinH,clockpinH,LSBFIRST,position); // shifts the data of dotmatrix row on shift register
     digitalWrite(latchpinH,HIGH);                  
     
    delay(2);//keep for 2ms

    cleardisplay(); // clear the display
    
    digitalWrite(latchpinV,LOW);
    shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<i));
    digitalWrite(latchpinV,HIGH);   
     
     digitalWrite(latchpinH,LOW);
     shiftOut(datapinH,clockpinH,LSBFIRST,ballposition);
     digitalWrite(latchpinH,HIGH);
 
    delay(2);
   }
   
   if(i == 6)
   {
     if(!(oldpositionbot & ballposition))
     {
    while(1)
      {
          digitalWrite(latchpinV,LOW);
          shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<0));
          digitalWrite(latchpinV,HIGH);   
            digitalWrite(latchpinH,LOW);
            shiftOut(datapinH,clockpinH,LSBFIRST,0b11111111);
            digitalWrite(latchpinH,HIGH);
            delay(2);
          end();
     }
     }
     
    if(oldpositionbot > newpositionbot)
      wedge = 3 ;
    else if(oldpositionbot < newpositionbot)
      wedge = 1 ;
    else
      wedge = 2 ;
     {
       if(wedge==2)
       ball=ball;
       else if(ball==2)
       ball=wedge;
       else
       ball=2;
     } 
   }
   if((ballposition == 0b10000000 )&& (ball==1))
   ball=3;
   else if((ballposition == 0b00000001) && (ball==3))
   ball=1;
   if(i!=1)
   {
    if(ball==1)
    ballposition=(ballposition<<1);
    if(ball==3)
    ballposition=(ballposition>>1);
   }
 } 
 
 for(i=1;i<7;i++)
 {
    for( t=0 ; t<speed; t++) 
    {
     cleardisplay();
     
     digitalWrite(latchpinH,LOW);
     position = analogit();

     if((i==1)&& (which == 1))
     newpositiontop=position;

     if((i==6) && (which == 0))
     oldpositionbot=position;
     shiftOut(datapinH,clockpinH,LSBFIRST,position);
     digitalWrite(latchpinH,HIGH);
     
    delay(2);

    cleardisplay();
    
    digitalWrite(latchpinV,LOW);
    shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<i));
    digitalWrite(latchpinV,HIGH);   
 
     digitalWrite(latchpinH,LOW);
     shiftOut(datapinH,clockpinH,LSBFIRST,ballposition);
     digitalWrite(latchpinH,HIGH);
 
    delay(2);
   }
    if(i == 1)
   {
     if(!(oldpositiontop & ballposition))
     {
          while(1)
      {
          digitalWrite(latchpinV,LOW);
          shiftOut(datapinV,clockpinV,MSBFIRST,~(0b00000001<<7));
          digitalWrite(latchpinV,HIGH);   
            digitalWrite(latchpinH,LOW);
            shiftOut(datapinH,clockpinH,LSBFIRST,0b11111111);
            digitalWrite(latchpinH,HIGH);
            delay(2);
          end();
     }

       
     }
     
     
    if(oldpositiontop > newpositiontop)
      wedge = 3 ;
    else if(oldpositiontop < newpositiontop)
      wedge = 1 ;
    else
      wedge = 2 ;
     
     {
       if(wedge==2)
       ball=ball;
       else if(ball==2)
       ball=wedge;
       else
       ball=2;
       
     } 
   }
   if((ballposition == 0b10000000) && (ball==1))
   ball=3;
   
   else if((ballposition == 0b00000001) && (ball==3))
   ball=1;
   
   if(i!=6)
   {
   if(ball==1)
   ballposition=(ballposition<<1);
   if(ball==3)
   ballposition=(ballposition>>1);
   }
}
 
 
 
} 



