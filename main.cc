#include <drogon/drogon.h>
int main() {
    //Load config file
    drogon::app().loadConfigFile("../config.yaml");
    drogon::app().run();
    return 0;
}