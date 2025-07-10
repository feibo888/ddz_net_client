#ifndef JOINROOM_H
#define JOINROOM_H

#include "infomation.pb.h"
#include <QDialog>

namespace Ui {
class JoinRoom;
}


enum class DialogType:char{Manual, Search};


class JoinRoom : public QDialog
{
    Q_OBJECT

public:
    explicit JoinRoom(DialogType type, QWidget *parent = nullptr);
    ~JoinRoom();

    void encodeMessage(RequestCode code);

    void searchRoom();
    void joinRoom();

private:
    Ui::JoinRoom *ui;
};

#endif // JOINROOM_H
