#include "gitinterface.h"

File::File(const char& _Stage,const char& _Status,const std::string& _FileName)
{
    switch (_Stage) {
        case 'A': m_Stage = Stage::staged; m_Status = Status::newfile; break;
        case 'M': m_Stage = Stage::staged; m_Status = Status::modified; break;
        default: m_Stage = Stage::unstaged; break;
    }
    switch (_Status) {
        case 'D': m_Stage = Stage::unstaged; m_Status = Status::deleted; break;
        case 'M': m_Stage = Stage::unstaged; m_Status = Status::modified;break;
        case ' ': ; break;
        default: m_Stage = Stage::unstaged; break;
    }
    m_FileName = _FileName;
}

GitInterface::GitInterface()
{

}

GitInterface::GitInterface(const std::string& _gitPath)
{
    setPath(_gitPath);
}

void GitInterface::init(const std::string& _gitPath)
{
    std::string command = "cd ";
    command += _gitPath;
    command += " && git init";
    exec(command.c_str());
    setPath(_gitPath);
}

//not shure on correct working  // _ProjectName must be same like repos name from _remoteLink
void GitInterface::clone(const std::string& _remoteLink, const std::string& _Folder, const std::string& _ProjectName)
{
    if(!_remoteLink.empty() && !_Folder.empty())
    {
        std::string command = "cd ";
        command += _Folder;
        command += " && git clone ";
        command += _remoteLink;
        exec(command.c_str());
        setPath(_Folder + '/'+ _ProjectName);
        UppdateInfo();
    }
}

void GitInterface::add()
{
    if(!m_GitPath.empty())
    {
        std::string command = "cd ";
        command += m_GitPath;
        command += " && git add .";
        exec(command.c_str());
        UppdateInfo();
    }
}

void GitInterface::commit(const std::string& _commitMessage)
{
    if(!m_GitPath.empty())
    {
        std::string command = "cd ";
        command += m_GitPath;
        command += " && git commit -m \"";
        command += _commitMessage;
        command += '\"';
        exec(command.c_str());
    }
}

void GitInterface::push()
{
    if(!m_GitPath.empty())
    {
        std::string command = "cd ";
        command += m_GitPath;
        command += " && git push origin ";
        for(auto& branch : m_Branches)
            if(branch.second)
                command += branch.first;
        exec(command.c_str());
    }
}

const std::list<File> GitInterface::getFileList()
{
    return m_Files;
}

std::string  GitInterface::geFileInfo(const std::string& _FileName)
{
     // git log -p -- "path\example.txt"  - selected file info in diff format
    std::string result = "File Error";
    if(!m_GitPath.empty())
    {
        std::string command = "cd ";
        command += m_GitPath;
        command += " && git log -p -- ";
        command += _FileName;
        result = exec(command.c_str());
    }
    return result;
}

const std::list<std::pair<std::string,bool>> GitInterface::getBranches()
{
    return m_Branches;
}

const std::string& GitInterface::getPath() const
{
    return m_GitPath;
}

const std::string& GitInterface::getInfo() const
{
    return m_Info;
}

void GitInterface::setPath(const std::string& _Path)
{
    m_GitPath = _Path;
    UppdateInfo();
}

void GitInterface::setBranch(const std::string& _Branch)
{
    if(!m_GitPath.empty())
    {
        std::string command = "cd ";
        command += m_GitPath;
        command += " && git checkout ";
        command += _Branch;
        exec(command.c_str());
        UppdateInfo();
    }
}

void GitInterface::UppdateInfo()
{
    //uppdate:
    //  std::list<std::string> m_Branches
    //  std::list<File> m_Files
    // -----command for info :
    // git branch - list of branches and selected branch;
    // git status -p - files: list, status and stage;


    if(!m_GitPath.empty())
    {
        std::string command = "cd ";
        command += m_GitPath;
        command += " && git log --reverse";
        m_Info = exec(command.c_str());

        //---------------------------------------------
        m_Branches.clear();
        command = "cd ";
        command += m_GitPath;
        command += " && git branch";
        std::string branches = exec(command.c_str());

        std::istringstream iss(branches);
        std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());
        bool isSelected = false;
        for(auto& branch : results){
            if(branch[0] == '*')
            {
                isSelected = true;
                continue;
            }
            m_Branches.push_back({branch,isSelected});
            isSelected = false;
        }
        //---------------------------------------------
        m_Files.clear();
        command = "cd ";
        command += m_GitPath;
        command += " && git status -s";
        std::string str_files = exec(command.c_str());
        command.clear();
        if(!str_files.empty()){
            const std::regex rdelim{"\n"};
            std::vector<std::string> files{
                std::sregex_token_iterator(str_files.begin(), str_files.end(), rdelim, -1),
                std::sregex_token_iterator()
                };
            for(auto& file:files)
                m_Files.push_back(File(file[0],file[1], std::string(file.begin()+3,file.end())));
        }
        //---------------------------------------------
    }
}

std::string GitInterface::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result = "";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
