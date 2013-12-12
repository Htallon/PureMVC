#include <iostream>
using namespace std;

#include "Facade.h"
#include "ICommand.h"
#include "Mediator.h"
using namespace Mvc::Patterns;
using namespace Mvc::Interface;

enum NOTIFY_TYPE
{
    NT_ONE,
    NT_TWO,
    NT_THREE,
    NT_TEST,
    NT_MAX
};

enum MEDIATOR_TYPE
{
    MT_ONE,
    MT_TWO,
    MT_MAX
};

IFacade* facade = NULL;

class MyMediator : public Mediator
{
public:
    MyMediator(IFacade* facade) : Mediator(facade)
    {
        m_mediatorName = MT_ONE;
    }

private:
    vector<Mvc::Interface::NOTIFICATION_NAME_TYPE>    ListNotificationInterests()
    {
        vector<Mvc::Interface::NOTIFICATION_NAME_TYPE> tempList;
        tempList.push_back(NT_ONE);
		tempList.push_back(NT_TWO);
        tempList.push_back(NT_TEST);
        return tempList;
    }

    void    HandlerNotification(INotification* pNotification)
    {
        if(NT_ONE == pNotification->getName())
        {
            cout << "handler NT noe" << endl;
        }
        else if(NT_TEST == pNotification->getName())
        {
            facade->RemoveMediator(MT_TWO);
        }
    }
};

class MyTwoMediator : public Mediator
{
public:
    MyTwoMediator(IFacade* facade) : Mediator(facade)
    {
        m_mediatorName = MT_TWO;
    }

private:
    vector<Mvc::Interface::NOTIFICATION_NAME_TYPE>    ListNotificationInterests()
    {
        vector<Mvc::Interface::NOTIFICATION_NAME_TYPE> tempList;
        tempList.push_back(NT_TWO);
        tempList.push_back(NT_TWO);
        tempList.push_back(NT_TEST);
        return tempList;
    }

    void    HandlerNotification(INotification* pNotification)
    {
        if(NT_TWO == pNotification->getName())
        {
            cout << "handler NT two, and nofity NT one" << endl;
            
            SendNotification(NT_ONE, NULL);
        }
    }
};

class MyCommand : public Mvc::Interface::ICommand
{
private:
    void    Execute(INotification*)
    {
        cout << "in my command" << endl;
    }
};

Mvc::Interface::ICommand* myCommandCreator()
{
    return new MyCommand;
}

int main()
{
    facade = new Mvc::Patterns::Facade;

    facade->RegisterMediator(new MyMediator(facade));
    facade->RegisterMediator(new MyTwoMediator(facade));
    
    // RegisterCommand �ڶ�����������Ϊ����ָ��,���ڿ���е��ô˻ص��Դ���������Command����
    // �ҵ������Ķ������Execute������,�ᱻ�ݻ�
    facade->RegisterCommand(NT_THREE, myCommandCreator);
    facade->RegisterCommand(NT_THREE, myCommandCreator);


    facade->SendNotification(NT_THREE, NULL);

    facade->SendNotification(NT_ONE, NULL);
    facade->SendNotification(NT_TWO, NULL);
    facade->SendNotification(NT_TEST, NULL);

    delete facade;

    cin.get();
    return 0;
}