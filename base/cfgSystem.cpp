#include "cfgSystem.hpp"
#include "math/math.hpp"
#include "Asserts.hpp"

#define charge ne_laz::SectionCharge::_instance
#define CFG_LINE_RESERVE			(128)

ConfigManager::ConfigManager() {
    mBaseFilePath = readPath(shared_str("config")) / "system.cnf";
    cnfLoad(mBaseFilePath);
    loadEnd();
}

void ConfigManager::loadEnd() const {
    charge().loadEnd();
}

void ConfigManager::cnfLoad(bfs::path curDir) const {


    //printf("file: %s\n", curDir.string().c_str());
    std::string file;		//readed

    readFile(file, curDir, std::ios::in | std::ios::binary);

    str_vec fileStrings;	//not null lines vector from file
    //fileStrings.reserve(140000);
    str_vec preprocessor;	//preprocessor tasks

    //copy not null lines, cutting ';' comments
    const char* file_ptr = file.c_str();
    while (*file_ptr != '\0') {
        std::string readed;

        skip_spaces(file_ptr);

        bool first_slash = false;

        //auto ptr = file_ptr;
        while (wo_0_n_r(*file_ptr) && *file_ptr != ';') {
            if (*file_ptr == '/') {
                if (first_slash) {
                    readed.pop_back();
                    break;
                }
                first_slash = true;
            }

            readed.push_back(*file_ptr++);
        }

        if (*file_ptr == ';' || *file_ptr == '/') {
            while (wo_0_n_r(*file_ptr)) ++file_ptr;
        }

        if (!readed.empty()) {
            fileStrings.push_back(readed);
        }
        while (*file_ptr == '\n' || *file_ptr == '\r') ++file_ptr;
        //std::cout << "readed: " << readed << std::endl;
    }

    //parse
    str_vec section;
    enum task { no_task = 1, reading_section };
    task state = no_task;


    for (U32 i = 0; i < fileStrings.size(); ++i) {
        switch (state) {
        case no_task:

            switch (fileStrings[i][0]) {

            case '#':
                preprocessor.push_back(fileStrings[i]);
                break;

            case '[':
                state = reading_section;
                section.push_back(fileStrings[i]);
                break;
            }
            break;

        case reading_section:
            while (fileStrings[i][0] != '[' && fileStrings[i][0] != '#' && i < fileStrings.size()) {
                section.push_back(fileStrings[i++]);
                if (i == fileStrings.size()) break;
            }
            i--;
            charge().parseSection(section);
            //std::cout << "section parse:" << section[0] << "\n";
            section.clear();
            state = no_task;
            break;
        }
    }

    //std::cout << "loaded: " << curDir.c_str() << std::endl;

    //preprocessor tasks
    for (auto& i : preprocessor) {
        std::string comand;
        std::string append;
        readDirective(comand, append, i);

        if (comand == "#include") {
            printf("%s", append.c_str());
            R_ASSERTF(append[0] == '"', "missing first [\"] in #include directive, file: \"%s\"", curDir.c_str());
            R_ASSERTF(append[append.length() - 1] == '"', "missing second [\"] in #include directive, file: \"%s\"", curDir.c_str());
            append.erase(0, 1);
            append.erase(append.size() - 1, 1);
            auto newFile = curDir.parent_path();
            newFile /= append.c_str();
            cnfLoad(newFile);
        }
    }

}

//parse preprocessor directive
void ConfigManager::readDirective(std::string& comand, std::string& append, const std::string& base) const {

    comand.clear();
    append.clear();

    U32 i = 0;

    const char* str = base.c_str();

    while(wo_space_tab_0_n_r(str[i]))
        comand.push_back(str[i++]);

    while (if_space_tab(str[i])) ++i;

    if (str[i] == '"') {
        append.push_back(str[i++]);
        while (str[i] != '"' && wo_space_tab_0_n_r(str[i])) {
            append.push_back(str[i++]);
        }
        append.push_back(str[i]);
    }
    else {
        while (wo_space_tab_0_n_r(str[i])) {
            append.push_back(str[i++]);
        }
    }
}

//for string with arguments like "aaa, bbb, ccc..." 
arguments_vec ConfigManager::readingArguments(shared_str str, char delim) {
    arguments_vec vec;
    const char* charStr = str.c_str();

    while (*charStr != '\0') {
        while (if_space_tab(*charStr)) { ++charStr; }
        std::string res;
        res.reserve(100);
        while (*charStr != delim && *charStr != ' ' && *charStr != '\t' && *charStr != '\0') {
            res.push_back(*charStr++);
        }
        if (res.length() != 0)
            vec.push_back(shared_str(res.c_str()));
        if (*charStr == delim)
            charStr++;
    }

    return vec;
}

//TODO: string '=' kill process
//for string pair like "aaa = bbb" 
name_value ConfigManager::readPair(const std::string &str, char delim) {
    //Log("read: %s", str.c_str());
    std::string left, right;
    left.reserve(100);
    right.reserve(100);

    const char* charStr = str.c_str();

    //read left value
    while (*charStr != delim && wo_space_tab_0_n_r(*charStr)) left.push_back(*charStr++);

    //skip spaces
    while (if_space_tab(*charStr)) ++charStr;

    //checking denumerator
    if (*charStr != delim) { return std::make_pair(shared_str(left.c_str()), shared_str()); }
    //R_ASSERTF(*charStr == delim, "missing denominator '%c' in string \"%s\"", delim, str.c_str());
    ++charStr;

    //skip spaces
    while (if_space_tab(*charStr)) ++charStr;

    //read right value. if first symbol is '"', reading before second '"'
    if (*charStr != '\0') {
        while (wo_0_n_r(*charStr)) right.push_back(*charStr++);
        while (if_space_tab(right[right.length() - 1])) right.pop_back();
    }

    return std::make_pair(shared_str(left.c_str()), shared_str(right.c_str()));
}

shared_str ConfigManager::base_shared_str(shared_str section, shared_str name) const {
    auto finding = charge().findSection(section);
    R_ASSERTF(finding != nullptr, "can't find section [%s]", section.c_str());
    return finding->getValue(name);
}

shared_str ConfigManager::r_string(shared_str section, shared_str name) const {
    shared_str str = base_shared_str(section, name);
    if (str[0] == '"' && str[str.length() - 1] == '"')
        str.erase_edge(0, str.length() - 1);
    return str;
}


bool ConfigManager::r_bool(shared_str section, shared_str name) const {
    shared_str read = base_shared_str(section, name);
    if (read == shared_str("true")) return true;
    else if (read == shared_str("false")) return false;
    R_ASSERTF(true, "wrong bool '%s' in section [%s]", name.c_str(), section.c_str());
    return false;
}

fixed ConfigManager::r_fixed(shared_str section, shared_str name) const {
    return fixed(base_shared_str(section, name).c_str());
}

double ConfigManager::r_double(shared_str section, shared_str name) const {
    return math::atof<double>(base_shared_str(section, name).c_str());
}

float ConfigManager::r_float(shared_str section, shared_str name) const {
    return math::atof<float>(base_shared_str(section, name).c_str());
}

S64 ConfigManager::r_s64(shared_str section, shared_str name) const {
    return math::atoi<S64>(base_shared_str(section, name).c_str());
}

U64 ConfigManager::r_u64(shared_str section, shared_str name) const {
    return math::atoui<U64>(base_shared_str(section, name).c_str());
}

S32 ConfigManager::r_s32(shared_str section, shared_str name) const {
    return math::atoi<S32>(base_shared_str(section, name).c_str());
}

U32 ConfigManager::r_u32(shared_str section, shared_str name) const {
    return math::atoui<U32>(base_shared_str(section, name).c_str());
}

S16 ConfigManager::r_s16(shared_str section, shared_str name) const {
    return math::atoi<S16>(base_shared_str(section, name).c_str());
}

U16 ConfigManager::r_u16(shared_str section, shared_str name) const {
    return math::atoui<U16>(base_shared_str(section, name).c_str());
}

S8 ConfigManager::r_s8(shared_str section, shared_str name) const {
    return math::atoi<S8>(base_shared_str(section, name).c_str());
}

U8 ConfigManager::r_u8(shared_str section, shared_str name) const {
    return math::atoui<U8>(base_shared_str(section, name).c_str());
}

bool ConfigManager::is_section_exist(shared_str section) const {
    return charge().findSection(section) != nullptr;
}

bool ConfigManager::is_value_exist(shared_str section, shared_str name) const {
    auto finding = charge().findSection(section);
    R_ASSERTF(finding != nullptr, "can't find section [%s]", section.c_str());
    return finding->isValueExist(name);
}

void ConfigManager::get_values_list(shared_str section, std::vector<shared_str>& list) const {
    auto finding = charge().findSection(section);
    R_ASSERTF(finding != nullptr, "can't find section [%s]", section.c_str());
    finding->get_values_list(list);
}

void ConfigManager::readFile(std::string& file, bfs::path path, std::ios_base::openmode mode, S32 prot, U32 reserve) {
    std::ifstream filestream(path.c_str(), mode);
    R_ASSERTF(filestream.is_open(), "can't find file \"%s\"", path.string().c_str());

    file.clear();
    file.reserve(reserve);
    while (!filestream.eof()) {
        char c;
        filestream.get(c);
        if (filestream.eof()) break;
        file.push_back(c);
    }
    filestream.close();
}

// Todo: double realisation

// Todo: Не сможет грузануть файл больший чем оператива. нужны ассерты
void ConfigManager::readFile(bytes_t& file, bfs::path path, std::ios_base::openmode mode, S32 prot, U32 reserve) {
    std::ifstream filestream(path.c_str(), mode | std::ios::binary);
    R_ASSERTF(filestream.is_open(), "can't find file \"%s\"", path.string().c_str());

    file.clear();

    // находим размер файла
    filestream.seekg(0, std::ios::end);
    auto end = filestream.tellg();
    filestream.seekg(0);
    auto start = filestream.tellg();

    auto size = end - start;

    file.reserve(size);
    file.resize(size);
    filestream.read(reinterpret_cast<char*>(const_cast<U8*>(file.data())), size);

    filestream.close();
}

bool ConfigManager::isFileExist(const bfs::path& path) const {
    std::ifstream filestream(path.c_str(), std::ios_base::binary);
    if (filestream.is_open()) {
        filestream.close();
        return true;
    }
    return false;
}


// Todo: усовершенствовать ридпас - читать пути при создании мэнэджера и хранить в мапе

bfs::path ConfigManager::readPath(shared_str path_name) {
    auto path = bfs::current_path().parent_path();      // getting this executable path
    auto filesys_path = path / FILESYSTEM_FILE;         // path to filesystem file

    // read file
    std::string file;
    readFile(file, filesys_path.string().c_str(), std::ios::in | std::ios::binary);

    ne_laz::section_map sectmap;		// map as [name] = value

    std::string right;  right.reserve(100);
    std::string left;   left.reserve(100);

    // start
    auto fileptr = file.c_str();
    while (true) {
        skip_spaces(fileptr);
        read_before_gap(left, fileptr); // first val
        skip_spaces(fileptr);

        R_ASSERTF(*fileptr == '=', "left %s missing operator '=' in \"%s\"", left.c_str(), filesys_path.string().c_str());
        ++fileptr;

        skip_spaces(fileptr);
        read_before_breakline(right, fileptr);	// second val
        truncate_spaces(right);

        sectmap[shared_str(left.c_str())] = shared_str(right.c_str());
        left.clear(); right.clear();

        skip_gaps(fileptr);
        if (*fileptr == '\0') break;
    }

    auto finding = sectmap.find(path_name);
    R_ASSERTF(finding != sectmap.end(), "can't find path \"%s\"", path_name.c_str());
    arguments_vec vals = readingArguments(finding->second, '+');

    for (auto& i : vals) {
        if (i.at(0) == '$') {
            i.erase_front(0);
            auto finding2 = sectmap.find(i);
            R_ASSERTF(finding2 != sectmap.end(), "can't find path \"%s\"", i.c_str());
            i = finding2->second;
        }
        path /= i.c_str();
    }
    return path;

}

#undef charge

namespace ne_laz {

    //add parent
    void Section::addParent(const shared_str& parentSect) {
        _parentSectionsQueue.push(parentSect);
    }

    //add pair "name = value"
    void Section::addValue(const shared_str& name, const shared_str& val) {
        _sectionMap[name] = val;
    }

    void Section::_info() const {
        for (const auto& i : _sectionMap) {
            printf("%s = %s\n", i.first.c_str(), i.second.c_str());
        }
    }

    //loading section after all parent sections
    void Section::reloadParents() {
        if (_parentSectionsQueue.empty())
            return;

        auto copy = _sectionMap;
        while (!_parentSectionsQueue.empty()) {
            auto name = _parentSectionsQueue.front();
            auto finding = SectionCharge::_instance().findSection(name);
            R_ASSERTF(finding != nullptr, "section [%s] loading from child [%s] does not exists", name.c_str(), _name.c_str());
            _parentSectionsQueue.pop();

            for (const auto& i : finding->_sectionMap) {
                _sectionMap[i.first] = i.second;
            }
        }

        for (const auto& i : copy) {
            _sectionMap[i.first] = i.second;
        }
    }

    shared_str Section::getValue(shared_str name) const {
        auto finding = _sectionMap.find(name);
        R_ASSERTF(finding != _sectionMap.end(), "can't find value '%s' in section [%s]", name.c_str(), _name.c_str());
        return finding->second;
    }

    bool Section::isValueExist(shared_str name) const {
        return _sectionMap.find(name) != _sectionMap.end();
    }

    void Section::get_values_list(std::vector<shared_str>& list) const {
        list.reserve(_sectionMap.size());
        for (const auto& i : _sectionMap) {
            list.push_back(i.first);
        }
    }

    SectionCharge::~SectionCharge() {
        for (auto& i : sections) {
            delete i.second;
        }
    }

    //parse section from strings vector
    void SectionCharge::parseSection(str_vec& sect) {
        if (sect.empty())
            return;

        //read section name
        auto* section = new Section;
        shared_str sectName = sect[0].c_str();
        sectName.erase_edge(sectName.find('['), sectName.find(']'));
        section->setName(sectName);

        //creating section
        auto finding = sections.find(sectName);
        R_ASSERTF(finding == sections.end(), "duplicate section [%s]", sectName.c_str());
        sections[sectName] = section;


        //reading parent section names
        sectName = sect[0].c_str();
        sectName.erase_front(sectName.find(']'));
        //parent sect exist
        if (sectName.find(':') != sectName.length()) {
            sectName.erase_front(sectName.find(':'));
            arguments_vec parentSections = ConfigManager::readingArguments(sectName);
            for (const auto& i : parentSections) {
                section->addParent(i);
            }
        }

        //data reading
        for (U32 i = 1; i < sect.size(); ++i) {
            name_value sectPair = ConfigManager::readPair(sect[i]);
            section->addValue(sectPair.first, sectPair.second);
        }
    }

    void SectionCharge::_info() const {
        for (auto& i : sections) {
            printf("[%s]\n", i.first.c_str());
            i.second->_info();
            printf("\n");
        }
    }

    void SectionCharge::loadEnd() {
        for (auto& i : sections) {
            i.second->reloadParents();
        }
    }

    Section* SectionCharge::findSection(const shared_str &sect) const {
        auto finder = sections.find(sect);
        if (finder == sections.end()) return nullptr;
        return finder->second;
    }
}



CFileReader::CFileReader(const bfs::path& path) {
    DLog("CFileReader:: open file at '%s'", path.string().c_str());
    _file = std::ifstream(path.c_str(), std::ios::binary);
    R_ASSERTF(_file.is_open(), "can't open file \"%s\"", path.string().c_str());

}

CFileReader::~CFileReader() {
    _file.close();
}



CFileWritter::CFileWritter(const bfs::path& path, bool append) {
// Todo: забацать создание пути
    DLog("CFileWritter:: create file at '%s'", path.string().c_str());

    auto mode = std::ios_base::out | std::ios_base::binary | (append ? std::ios_base::app : std::ios_base::openmode(0));
    _file = std::ofstream(path.c_str(), mode);

    DLog("CFileWritter:: file creating '%s'", path.string().c_str());
    R_ASSERTF(_file.is_open(), "can't create file \"%s\"", path.string().c_str());
}


CFileWritter::~CFileWritter() {
    _file.close();
}