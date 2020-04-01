#include "data.h"



//-----Data input methods----------------------------------------------------------------------
    DataIn::DataIn(int count):count(count),countTry(count)
    {}
    void DataIn::subscribe(Observer *obs)
    {
        subs.push_back(obs);
    }

    void DataIn::checkDilimiter(std::string& str)
    {
        if (str == "{")
        {
            if(checkD.first) ++checkD.second;
            else
            {
                bulk.first.clear();
                checkD.first = true;
                ++checkD.second;
            }
            
        }
        else if (str == "}")
        {
            if (checkD.second) --checkD.second;
        }
    }

    void DataIn::setData(std::string&& str) 
    {
        checkDilimiter(str);
        if(checkD.first)
        {
            if (str!="{" && str!="}")
            {
                if(bulk.first.size() == 0) 
                {
                    bulk.second = std::chrono::seconds(std::time(NULL));
                }
                bulk.first.emplace_back(str);
            }
            else if (!checkD.second)
            {
                notify();
            }
        }
        else
        {
            if (str!="{" && str!="}" && countTry)
            {
                if(bulk.first.size() == 0)
                {
                    bulk.second = std::chrono::seconds(std::time(NULL));
                }
                bulk.first.emplace_back(str);
                --countTry;
            }
            if(!countTry)
            {
                notify();
            }
        }
        
    }

    void DataIn::notify() 
    {
        for (auto s : subs) 
        {
            s->update(bulk);
        }
        clearData();
    }

    void DataIn::clearData()
    {
        bulk.first.clear();
        checkD.first = false;
        checkD.second = 0;
        countTry = count;
    }

    int DataIn::getQuantity()
    {
        return bulk.first.size();
    }


//-----Data to console methods-------------------------------------------------------------------
    DataToConsole::DataToConsole(DataIn *data) 
    {
        data->subscribe(this);
    }

    void DataToConsole::update(Bulk bulk)
    {
        std::cout << "bulk: ";
        for(auto& str : bulk.first)
        {
            if(str==bulk.first[0]) std::cout << str;
            else std::cout << ", " << str;
        }
        std::cout << std::endl;
    }


//-----Data to file methods-----------------------------------------------------------------------
    DataToFile::DataToFile(DataIn *data) 
    {
        data->subscribe(this);
    }

    void DataToFile::update(Bulk bulk)
    {
        std::ofstream out;
        auto timeUNIX = bulk.second.count();
        std::string path = "bulk"+ std::to_string(timeUNIX) + ".log";
        out.open(path);
        if (out.is_open(),std::ios::app)
        {
            out << "bulk: ";
            for(auto& str : bulk.first)
            {
                if(str==bulk.first[0]) out << str;
                else out << ", " << str;
            }
        }
        out.close();
    }
