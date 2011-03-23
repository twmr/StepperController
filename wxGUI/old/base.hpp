#ifndef __BASIC_HPP__
#define __BASIC_HPP__

class HalloWeltApp : public wxApp {
   public:
   virtual bool OnInit();
};

class BasicFrame : public wxFrame {
   public:
   BasicFrame( const wxChar *title,
               int xpos, int ypos,
               int width, int height);
   ~BasicFrame();
};

#endif
