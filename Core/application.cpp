#include "application.h"

namespace Core {

Application::Application(AppNode *root, QObject *parent)
    : QObject(parent)
{
    nodes.append(root);
}

} // namespace Core
