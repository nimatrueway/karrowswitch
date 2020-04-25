// c daemon
#include <unistd.h>

// functional tools
#include <functional>
using std::function;

// multi-threading
#include <mutex>
using std::mutex;

// io
#include <iostream>
using std::cout, std::endl, std::string;

// signal
#include <csignal>
using std::signal;

// x11
#include <X11/Xlib.h>
#include <X11/extensions/record.h>
#include <X11/extensions/XTest.h>
#include <X11/XKBlib.h>

// ------------------------------------------------- GLOBALS

bool DEBUG;

// ------------------------------------------------- X11/XTEST ENGINE

namespace X11Handler {
    XRecordRange *record_range_spec;
    Display *ctrl_conn;
    Display *data_conn;
    XRecordContext record_ctx;
    function<void (KeyCode, bool)> user_callback;
    mutex wait_for_sigterm_sigint{};
    void exit_handler(int signal) {
      if (DEBUG)
        cout << "Received interrupt/termination signal! (" << signal << ")" << endl;
      XLockDisplay (ctrl_conn);
      XRecordDisableContext(ctrl_conn, record_ctx);
      XSync(ctrl_conn, False);
      XUnlockDisplay (ctrl_conn);
      wait_for_sigterm_sigint.unlock();
    }
    void parent_handler(XPointer usr_data, XRecordInterceptData* data) {
      XLockDisplay(ctrl_conn);
      if (data->category == XRecordFromServer) {
        int key_event = data->data[0];
        KeyCode key_code  = data->data[1];
        if (DEBUG) {
          auto event = std::to_string(key_event);
          if (key_event == KeyPress)
            event = "PRESSED";
          else if (key_event == KeyRelease)
            event = "RELEASED";
          cout << "Intercepted key event " << event << ", key code " << (int) key_code << endl;
        }
        user_callback(key_code, key_event == KeyPress);
      }
      XUnlockDisplay(ctrl_conn);
      XRecordFreeData(data);
    }
    void send( KeyCode key, bool isPressed) {
      XTestFakeKeyEvent(ctrl_conn, key, isPressed, 0);
    }
    void flush() {
      XFlush(ctrl_conn);
    }
    void init(function<void (int, bool)>&& callback) {
      int dummy, retcode;
      user_callback = callback;
      XRecordClientSpec client_spec = XRecordAllClients;
      record_range_spec = XRecordAllocRange();
      record_range_spec->device_events.first = KeyPress;
      record_range_spec->device_events.last = KeyRelease;
      retcode = XInitThreads();
      if (!retcode) // check
      {
        cout << "Failed to initialize threads." << endl;
        exit (EXIT_FAILURE);
      }
      data_conn = XOpenDisplay(nullptr);
      ctrl_conn = XOpenDisplay(nullptr);
      if (!(data_conn || ctrl_conn)) // check
      {
        cout << "Unable to connect to X11 display. Is $DISPLAY set?" << endl;
        exit (EXIT_FAILURE);
      }
      retcode = XQueryExtension(ctrl_conn, "XTEST", &dummy, &dummy, &dummy);
      if (!retcode) // check
      {
        cout << "Xtst extension missing" << endl;
        exit (EXIT_FAILURE);
      }
      retcode = XRecordQueryVersion(ctrl_conn, &dummy, &dummy);
      if (!retcode) // check
      {
        cout << "Failed to obtain xrecord version" << endl;
        exit (EXIT_FAILURE);
      }
      retcode = XkbQueryExtension(ctrl_conn, &dummy, &dummy, &dummy, &dummy, &dummy);
      if (!retcode)
      {
        cout << "Failed to obtain xkb version" << endl;
        exit (EXIT_FAILURE);
      }
      record_ctx = XRecordCreateContext(ctrl_conn, 0, &client_spec, 1, &record_range_spec, 1);
      if (record_ctx == 0) // check
      {
        cout << "Failed to create xrecord context" << endl;
        exit (EXIT_FAILURE);
      }
      XSync(ctrl_conn, False);
      retcode = XRecordEnableContext (data_conn, record_ctx, parent_handler, nullptr);
      if (!retcode)
      {
        cout << "Failed to enable xrecord context" << endl;
        exit (EXIT_FAILURE);
      }
      // wait for exit signals
      signal(SIGINT, &exit_handler);
      signal(SIGTERM, &exit_handler);
      wait_for_sigterm_sigint.lock();
      // free resources
      retcode = XRecordFreeContext(ctrl_conn, record_ctx);
      if (!retcode)
      {
        cout << "Failed to free xrecord context" << endl;
      }
      XFree (record_range_spec);
      XCloseDisplay(ctrl_conn);
      XCloseDisplay(data_conn);
    }
}

// ------------------------------------------------- APPLICATION LOGIC

const int STATE_IDLE = 0;
const int STATE_CONTROL_DOWN = 1;
const int STATE_PRESSED_ONCE = 2;

const KeySym KEY_CONTROL_LEFT_DOWN = 37;
const KeySym KEY_CONTROL_RIGHT_DOWN = 105;
const KeySym KEY_INSERT = 118;
const KeySym KEY_PAUSE = 127;

int state = STATE_IDLE;

void emulate_insert() {
  X11Handler::send(KEY_INSERT, true);
  X11Handler::send(KEY_INSERT, false);
  X11Handler::send(KEY_INSERT, true);
  X11Handler::send(KEY_INSERT, false);
  X11Handler::flush();
}

void handle(KeyCode key, bool isPressed) {
  auto isCtrl = key == KEY_CONTROL_LEFT_DOWN || key == KEY_CONTROL_RIGHT_DOWN;
  if (isCtrl) {
    if (state == STATE_IDLE && isPressed) {
      if (DEBUG)
        cout << "Ctrl has been pressed [state=1]." << endl;
      state = STATE_CONTROL_DOWN;
    } else if (!isPressed) {
      if (DEBUG)
        cout << "Ctrl has been released [state=0]." << endl;
      state = STATE_IDLE;
    }
    return;
  }
  if (key == KEY_PAUSE && !isPressed) {
    if (state == STATE_CONTROL_DOWN) {
      if (DEBUG)
        cout << "Ctrl+Pause has been pressed [state=2]" << endl;
      state = STATE_PRESSED_ONCE;
    } else if (state == STATE_PRESSED_ONCE) {
      if (DEBUG)
        cout << "Ctrl+Pause has been pressed twice! [state=1]" << endl;
      state = STATE_CONTROL_DOWN;
      emulate_insert();
    }
  }
}

int main (int argc, char *argv[]) {
  // parse args
  if (argc == 0) {
    cout << "Usage: pause-change-fixer [-d]";
    cout << "  Runs as a daemon unless -d flag is set" << endl;
    return 0;
  }
  DEBUG = false;
  for (int i = 0; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-d")
      DEBUG = true;
  }
  // go !
  if (!DEBUG)
    daemon(0, 0);
  X11Handler::init(handle);
}
