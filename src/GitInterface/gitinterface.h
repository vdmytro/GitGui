#ifndef GITINTERFACE_H
#define GITINTERFACE_H

#include <QDir>
#include <iostream>
#include <list>
#include <string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <regex>

enum Stage
{
    unstaged, staged
};

enum Status
{
    modified,  newfile,  deleted
};

struct File
{
    File(const char& _Stage,const char& _Status,const std::string& _FileName);
    std::string m_FileName;
    Stage m_Stage;
    Status m_Status;
};

class GitInterface
{
public:
    GitInterface();

    GitInterface(const std::string& _gitPath);

    void init(const std::string& _gitPath);

    void clone(const std::string& _remoteLink, const std::string& _Folder, const std::string& _ProjectName);

    void add();

    void commit(const std::string& _commitMessage);

    void push();

    const std::list<File> getFileList();

    std::string geFileInfo(const std::string& _FileName);

    const std::list<std::pair<std::string,bool>> getBranches();

    const std::string& getPath() const;

    const std::string& getInfo() const;

    void setPath(const std::string& _Path);

    void setBranch(const std::string& _Branch);

    //parsing info from git commands and uppdate class GitInterface
    void UppdateInfo();

private:

    std::string m_GitPath = "";

    std::list<std::pair<std::string,bool>> m_Branches;

    std::list<File> m_Files;

    //different information that user required
    std::string m_Info = "";

    //execute commands and get result;
    std::string exec(const char* cmd);
};

#endif // GITINTERFACE_H
