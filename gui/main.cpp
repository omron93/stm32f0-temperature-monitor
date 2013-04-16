#include "main.h"

#include <glibmm.h>

using namespace std;

bool set = false;

Gtk::SpinButton* button1 = 0;
Gtk::SpinButton* button2 = 0;
Gtk::SpinButton* button3 = 0;

Gtk::Label* temperature = 0;

int timer_number = 0;

fstream* ffout;

int main(int argc, char* argv[])
  {
    Gtk::Window* window = 0;
    Gtk::Button* config = 0;

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "mas.temperature.monitor");

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("gui.glade");
    builder->get_widget("window1", window);

    builder->get_widget("teplota1", button1);
    builder->get_widget("teplota2", button2);
    builder->get_widget("teplota3", button3);

    builder->get_widget("button1", config);
    builder->get_widget("aktual", temperature);

    button1->set_range(-55, 125);
    button1->set_increments(1, 10);
    button2->set_range(-55, 125);
    button2->set_increments(1, 10);
    button3->set_range(-55, 125);
    button3->set_increments(1, 10);

    config->signal_clicked().connect(sigc::ptr_fun(settemp));

    sigc::slot<bool> slot = sigc::ptr_fun(&gettemp);
    Glib::signal_timeout().connect(slot, 1000);


    system("stty -F /dev/ttyUSB0 0:4:cbd:8a00:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0");
    system("stty -F /dev/ttyUSB0 0:4:cbd:8a00:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0");

    ffout = new fstream("/dev/ttyUSB0");
    char temp;

    ffout->put(GETTEMP);
    ffout->get(temp);
    ffout->close();


    cout << "Teplota: " << dec << (int)temp << endl;
    stringstream text;
    text << int(temp);
    temperature->set_text(text.str());

    button1->set_value(15);
    button2->set_value(25);
    button3->set_value(35);

    settemp();

    app->run(*window);

    delete ffout;

    return 0;
  }

void settemp()
  {
      char x1 = button1->get_value();
      char x2 = button2->get_value();
      char x3 = button3->get_value();
      cout << "Nastaveno: " << dec << (int)x1 << " " << (int)x2 << " " << (int)x3 << endl;

      ffout->open("/dev/ttyUSB0");
      ffout->put(SETVALUES);
      ffout->close();

      ffout->open("/dev/ttyUSB0");
      ffout->put(x1);
      ffout->close();

      ffout->open("/dev/ttyUSB0");
      ffout->put(x2);
      ffout->close();

      ffout->open("/dev/ttyUSB0");
      ffout->put(x3);
      ffout->close();

  }

bool gettemp()
  {
    char temp;
    ffout->open("/dev/ttyUSB0");
    ffout->put(GETTEMP);
    ffout->get(temp);
    ffout->close();

    cout << "Teplota: " << dec << (int)temp << endl;

    stringstream text;
    text << int(temp);
    temperature->set_text(text.str());
    return true;
  }
