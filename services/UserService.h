#include "ServiceBase.h"

class UserService : public ServiceBase<UserService>
{
public:
    void sayHello() {
        LOG_INFO << "Hello from UserService!";
    }
};