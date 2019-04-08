
#include "Menu.h"
#include <string.h>
#include <stdio.h>

//_________________________ MenuItem : IMenu ________________________________________

MenuItem::MenuItem(const char * name, char * firstStr, char * secStr)
	: _name(name),
	_parrent(this),
	_nextElement(this),
	_prevElement(this),
	_firstStr(firstStr),
	_secStr(secStr)
{
}

//scroll menuItem right
void MenuItem::right()
{
}

//scroll menuItem left
void MenuItem::left()
{
}

//open menu item or submenu
void MenuItem::open(IMenu** pointer)
{
	if (_parrent) {
		*pointer = _parrent;
		_parrent->show();
	}
}

//show selected item name
void MenuItem::show()
{
	strcpy(_firstStr, "");
	strcat(_firstStr, _name);
	strcpy(_secStr, ">");
}

//show menu name
void MenuItem::showName()
{
	strcpy(_firstStr,">");
	strcat(_firstStr, _name);
	strcpy(_secStr, "");
}

//return pointer to next element in list
void MenuItem::setNext(IMenu * element)
{
	_nextElement = element;
}
//return pointer to prev element in list
void MenuItem::setPrev(IMenu * element)
{
	_prevElement = element;
}

void MenuItem::setParrent(IMenu* parrent)
{
	_parrent = parrent;
}

void MenuItem::fromBegin()
{}

//return pointer to next menu in list
IMenu* MenuItem::getNext()
{
	return _nextElement;
}

//return pointer to prev menu in list
IMenu* MenuItem::getPrev()
{
	return _prevElement;
}
//___________________________________________________________________________________

//________________________ MenuItemBool : MenuItem _______________________
MenuItemBool::MenuItemBool(const char * name, char * firstStr, char * secStr, bool * data, const char * trueState = "true", const char * falseState = "false")
	: MenuItem(name, firstStr, secStr),
	data(data),
	_trueState(trueState),
	_falseState(falseState)
{

}
void MenuItemBool::right()
{
	*data = !(*data);
	show();
}
void MenuItemBool::left()
{
	right();
}
void MenuItemBool::show()
{
	strcpy(_firstStr, _name);
	strcpy(_secStr, ">");
	if (*data) {
		strcat(_secStr, _trueState);
	}
	else {
		strcat(_secStr, _falseState);
	}
}
void MenuItemBool::showName()
{
	strcpy(_firstStr, ">");
	strcat(_firstStr, _name);
	if (data) {
		strcpy(_secStr, _trueState);
	}
	else {
		strcpy(_secStr, _falseState);
	}
}
//________________________________________________________________________

//_________________________ MenuItemInt : MenuItem _______________________
MenuItemInt::MenuItemInt(const char* name, char * firstStr, char * secStr, int * data, int dataMax, int dataMin, const char * pref = "")
	: MenuItem(name, firstStr, secStr),
	_data(data),
	_dataMax(dataMax),
	_dataMin(dataMin),
	_pref(pref)
{

}
void MenuItemInt::right()
{
	if (*_data == _dataMax) return;
	*_data = *_data + 1;
	show();
}
void MenuItemInt::left()
{
	if (*_data == _dataMin) return;
	*_data = *_data - 1;
	show();
}
void MenuItemInt::show()
{
	strcpy(_firstStr, _name);
	strcpy(_secStr, ">");
	char temp[20];
	sprintf(temp, "%d", *_data);
	strcat(_secStr, temp);
	strcat(_secStr, _pref);
}
void MenuItemInt::showName() 
{
	strcpy(_firstStr, ">");
	strcat(_firstStr, _name);
	sprintf(_secStr,"%d",*_data);
	strcat(_secStr, _pref);
}

//________________________________________________________________________

//______________________ MnuItemFloat : MenuItem _________________________
MenuItemFloat::MenuItemFloat(const char* name, char * firstStr, char * secStr, float * data, float  dataMax, float dataMin, float plusData,const char * pref = "")
	: MenuItem(name, firstStr, secStr),
	_data(data),
	_plusData(plusData),
	_dataMax(dataMax),
	_dataMin(dataMin),
	_pref(pref)
{}
void MenuItemFloat::right()
{
	if (*_data >= _dataMax)
	{
		*_data = _dataMax;
		return;
	}
	*_data = *_data + _plusData;
	show();
}
void MenuItemFloat::left()
{
	if (*_data <= _dataMin)
	{
		*_data = _dataMin;
		return;
	}
	*_data = *_data - _plusData;
	show();
}
void MenuItemFloat::show()
{
	int tempInt = ((int)(*_data * 10)) % 10;
	strcpy(_firstStr, _name);
	strcpy(_secStr, ">");
	char temp[20];
	sprintf(temp, "%d.%d", (int)*_data, tempInt);
	strcat(_secStr, temp);
	strcat(_secStr, _pref);
}
void MenuItemFloat::showName()
{
	int tempInt = ((int)(*_data * 10)) % 10;
	strcpy(_firstStr, ">");
	strcat(_firstStr, _name);
	sprintf(_secStr, "%d.%d", (int)*_data, tempInt);
	strcat(_secStr, _pref);
}
//________________________________________________________________________

//_______________________ MenuItemEvent : MenuItem _______________________
MenuItemEvent::MenuItemEvent(const char* name, char * firstStr, char * secStr, void(*event)(void), const char * pref = "do..")
	: MenuItem(name, firstStr, secStr),
	_event(event),
	_pref(pref)
{

}
void MenuItemEvent::open(IMenu** pointer) 
{
	(*_event)();
	MenuItem::open(pointer);
}
void MenuItemEvent::show()
{
	strcpy(_firstStr, _name);
	strcpy(_secStr, ">");
	strcat(_secStr, _pref);
}
void MenuItemEvent::showName()
{
	strcpy(_firstStr, ">");
	strcat(_firstStr, _name);
	strcpy(_secStr, "");
}
//________________________________________________________________________


//_________________________ Menu : IMenu _________________________________
/* class Menu members implementation */
Menu::Menu(const char* name, char * firstStr, char * secStr)
	: IMenu(),
	back("..back",firstStr,secStr),
	_name(name),
	_parrent(this),
	_curItem(&back),
	_firstItem(&back),
	_lastItem(&back),
	_nextElement(this),
	_prevElement(this),
	_firstStr(firstStr),
	_secStr(secStr)
{
}

//scroll menu right
void Menu::right()
{
	_curItem = _curItem->getNext();
	_curItem->showName();
}

//scroll menu left
void Menu::left()
{
	_curItem = _curItem->getPrev();
	_curItem->showName();
}

//open menu item or submenu
void Menu::open(IMenu** pointer)
{
	if (_curItem == &back)
	{
		*pointer = _parrent;
		_parrent->show();
		return;
	}
	*pointer = _curItem;
	_curItem->fromBegin();
	_curItem->show();
}

//show selected item name
void Menu::show()
{
	_curItem->showName();
}

//show menu name
void Menu::showName()
{
	
}

//add new item or submenu to menu
void Menu::addItem(IMenu* item) {
	_lastItem->setNext(item);
	_firstItem->setPrev(item);
	item->setNext(_firstItem);
	item->setPrev(_lastItem);
	item->setParrent(this);
	_lastItem = item;
}

//return pointer to next element in list
void Menu::setNext(IMenu * element)
{
	_nextElement = element;
}

//return pointer to prev element in list
void Menu::setPrev(IMenu * element)
{
	_prevElement = element;
}

//set parrent to go back in ierarhi
void Menu::setParrent(IMenu* parrent)
{
	_parrent = parrent;
}

//set first_item as cur_item
void Menu::fromBegin()
{
	_curItem = _firstItem;
}

//return pointer to next menu in list
IMenu* Menu::getNext()
{
	return _nextElement;
}

//return pointer to prev menu in list
IMenu* Menu::getPrev()
{
	return _prevElement;
}
//___________________________________________________________________________________




//_________________________ MainScreen _________________________________

MainScreen::MainScreen(Menu * menu, char * firstStr, char * secStr)
	: IMenu(),
	menu(menu),
	_firstStr(firstStr),
	_secStr(secStr)
{
	menu->setParrent(this);
}

void MainScreen::right()
{}

void MainScreen::left()
{}

void MainScreen::open(IMenu** pointer)
{
	*pointer = menu;
	menu->show();
}


void MainScreen::show()
{
	
	strcpy(_firstStr, "  <<<|Menu|>>>");
	strcpy(_secStr, "                  ");
	
}

void MainScreen::showName()
{}

void MainScreen::addItem(IMenu* item)
{}

void MainScreen::setNext(IMenu* element)
{}

void MainScreen::setPrev(IMenu* element)
{}

void MainScreen::setParrent(IMenu* parrent)
{}

void MainScreen::fromBegin()
{}

IMenu* MainScreen::getNext()
{
	return this;
}

IMenu* MainScreen::getPrev()
{
	return this;
}

//___________________________________________________________________________________
