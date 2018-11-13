#ifndef  CFG_SYSTEM
#define CFG_SYSTEM

#include <unordered_map>
#include <vector>
#include <queue>
#include <experimental/filesystem>
#include "engineBaseTypes.hpp"
#include "engineDefines.hpp"
#include "timer.hpp"

namespace bfs = std::experimental::filesystem;

typedef std::vector<std::string>					str_vec;
typedef std::vector<shared_str>						arguments_vec;
typedef std::pair<shared_str, shared_str>			name_value;

#define charge						ne_laz::SectionCharge::_instance
#define wo_space_tab_0_n_r(val)		((val) != ' ' && (val) != '\t' && (val) != '\0' && (val) != '\n' && (val) != '\r')
#define wo_0_n_r(val)				((val) != '\0' && (val) != '\n' && (val) != '\r')
#define if_space_tab_n_r(val)		((val) == ' ' || (val) == '\t' || (val) == '\n' || (val) == '\r')
#define if_space_tab(val)			((val) == ' ' || (val) == '\t')


// skipping spaces in char* str
#define skip_spaces(ptr_val)                while (if_space_tab(*(ptr_val))) ++(ptr_val)

// skipping spaces or line breaks before any text or null terminator
#define skip_gaps(ptr_val)                  while (if_space_tab_n_r(*(ptr_val))) ++(ptr_val);

// reading symbols from char* string to std::string before any gap
#define read_before_gap(dst, ptr_src)       while (wo_space_tab_0_n_r(*(ptr_src))) { (dst).push_back(*(ptr_src)); ++(ptr_src); }

// reading symbols from char* string to std::string before line breaks with spaces
#define read_before_breakline(dst, ptr_src) while (wo_0_n_r(*(ptr_src))) { (dst).push_back(*(ptr_src)); ++(ptr_src); }

// truncate spaces from back in std::string. STRING CAN'T CONSIST OF SPACES ONLY
#define truncate_spaces(std_str)            while (if_space_tab((std_str)[(std_str).length() - 1])) (std_str).pop_back();

namespace ne_laz {

    typedef std::unordered_map	<shared_str, shared_str>	section_map;
    typedef std::queue			<shared_str>				parent_queue;

    //////////////////// SECTION CLASS /////////////////////////////
    //
    //  Provide access to one section data
    //
    ////////////////////////////////////////////////////////////////
    class Section {
    private:
        parent_queue		_parentSectionsQueue;
        section_map			_sectionMap;
        shared_str			_name;

    public:
        void	reloadParents		();
        void	addParent           (const shared_str &parentSect);
        void	addValue            (const shared_str &name, const shared_str &val);
        shared_str getValue			(shared_str name) const;
        bool	isValueExist		(shared_str name) const;
        void	get_values_list		(std::vector<shared_str>& list) const;
        void	setName				(const shared_str &name)	{ _name = name; }

    //TODO: delete in release
    public:
        void	_info				() const;
    };



    typedef std::unordered_map	<shared_str, Section*>		value_map;
    class SectionCharge {

    private:
        value_map	sections;

    private:
        SectionCharge				() = default;
        ~SectionCharge				();
    public:
        SectionCharge				(const SectionCharge&) = delete;
        SectionCharge& operator=	(const SectionCharge&) = delete;

    public:
        void		loadEnd			();
        void		parseSection	(str_vec& sect);
        Section*	findSection(const shared_str &sect) const;

        static SectionCharge& _instance()		{ static SectionCharge instance; return instance; }

    //TODO: delete in release
    public:
        void		_info			() const;
    };
}


#define CFG_FILE_RESERVE			(1024 * 32)

class ConfigManager {

private:
    bfs::path				mBaseFilePath;

private:
    ConfigManager							();
    ~ConfigManager							() = default;
public:
    ConfigManager							(const ConfigManager&) = delete;
    ConfigManager&			operator=		(const ConfigManager&) = delete;

    void					loadEnd			() const;
    void					cnfLoad			(bfs::path curDir) const;
    void					readDirective	(std::string& comand, std::string& append, const std::string& base) const;

public:

    static void				readFile		(std::string& file, bfs::path path, std::ios_base::openmode mode = std::ios_base::in, S32 prot = 64, U32 reserve = CFG_FILE_RESERVE);
    static void				readFile		(bytes_t& file, bfs::path path, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary, S32 prot = 64, U32 reserve = CFG_FILE_RESERVE);
    bool					isFileExist		(const bfs::path& path) const;
    static bfs::path		readPath		(shared_str path_name);
    static arguments_vec	readingArguments(shared_str str, char delim = ',');
    static name_value		readPair		(const std::string &str, char delim = '=');
    shared_str				base_shared_str	(shared_str section, shared_str name) const;
    shared_str				r_string		(shared_str section, shared_str name) const;
    bool					r_bool			(shared_str section, shared_str name) const;
    fixed					r_fixed			(shared_str section, shared_str name) const;
    float					r_float			(shared_str section, shared_str name) const;
    double					r_double		(shared_str section, shared_str name) const;
    S64						r_s64			(shared_str section, shared_str name) const;
    U64						r_u64			(shared_str section, shared_str name) const;
    S32						r_s32			(shared_str section, shared_str name) const;
    U32						r_u32			(shared_str section, shared_str name) const;
    S16						r_s16			(shared_str section, shared_str name) const;
    U16						r_u16			(shared_str section, shared_str name) const;
    S8						r_s8			(shared_str section, shared_str name) const;
    U8						r_u8			(shared_str section, shared_str name) const;
    bool					is_section_exist(shared_str section) const;
    bool					is_value_exist	(shared_str section, shared_str name) const;
    void					get_values_list (shared_str section, std::vector<shared_str>& list) const;

    //universal func for numbers
    template<typename T>
    T						r_number(const shared_str &section, const shared_str &name) const {
        return				math::atodigit<T>(base_shared_str(section, name).c_str());
    }

    template<typename T>
    std::vector<T>			r_array(const shared_str &section, const shared_str &name) const {
        auto str_values		= base_shared_str(section, name);
        auto vec_values		= readingArguments(str_values);
        std::vector<T>		returning_vec;
        for (auto& i : vec_values)
            returning_vec.push_back(math::atodigit<T>(i.c_str()));
        return returning_vec;
    }

#ifdef CFG_SYSTEM_DEBUG
    void					_read_test_dictionary()
    {
        Log("Loading test dictionary");
        printf("Loading test dictionary\n");
        auto path = readPath(shared_str("test_dictionary"));

        auto tmstmp = zog::timer().timestamp();
        cnfLoad(path);
        auto elapsed = (zog::timer().timestamp() - tmstmp).sec();

        Log("Completed. Load time: %lf sec.", elapsed);
        printf("Completed. Load time: %lf sec.\n", elapsed);
#ifdef SHARED_DEBUG
        shared_str::_print_shared_map();
        shared_str::_memory_usability();
        sharedAlloc._infoMemoryBlocks();
#endif // SHARED_DEBUG
    }
#endif // CFG_SYSTEM_DEBUG

    static ConfigManager& _instance			() { static ConfigManager instance; return instance; }
};




class CFileReader {
protected:
    std::ifstream _file;

public:
    explicit CFileReader(const bfs::path& path);
    ~CFileReader();


// Todo: ассерт на вылет за границы файла

    template<typename T, size_t S1, size_t S2>
    void Read(T(&data)[S1][S2]) {
        DLog("CFileReader:: reading array [%i elements] [%i bytes] from [%p]", S1 * S2, S1 * S2 * sizeof(T), &_file);

        size_t size = S1 * S2 * sizeof(T);
        auto ptr = reinterpret_cast<char*>(&data[0]);
        _file.read(ptr, size);

        //size_t cur_pos = _file.tellg() + size;
        //_file.seekg(cur_pos);

        DLog("CFileReader:: array has been read [%i bytes] from [%p]", S1 * S2 * sizeof(T), &_file);
    }

    template<typename T, size_t S>
    void Read(T(&data)[S]) {
        DLog("CFileReader:: reading array [%i elements] [%i bytes] from [%p]", S, S * sizeof(T), &_file);

        size_t size = S * sizeof(T);
        auto ptr = reinterpret_cast<char*>(&data[0]);
        _file.read(ptr, size);

        //size_t cur_pos = _file.tellg() + size;
        //_file.seekg(cur_pos);
        DLog("CFileReader:: array has been read [%i bytes] from [%p]", S * sizeof(T), &_file);
    }

    template<typename T>
    void Read(T* data, size_t size_array) {
        DLog("CFileReader:: reading array [%i elements] [%i bytes] from [%p]", size_array, sizeof(T), size_array * sizeof(T), &_file);

        size_array *= sizeof(T);
        auto ptr = reinterpret_cast<char*>(data);
        _file.read(ptr, size_array);

        //size_t cur_pos = _file.tellg() + size;
        //_file.seekg(cur_pos);
        DLog("CFileReader:: array has been read [%i bytes] from [%p]", size * sizeof(T), &_file);
    }

    template<typename T>
    void Read(T& value) {
        DLog("CFileWritter:: writing value [%i bytes] to [%p]", sizeof(T), &_file);
        auto ptr = reinterpret_cast<char*>(&value);
        _file.read(ptr, sizeof(T));

        //size_t cur_pos = _file.tellg() + sizeof(T);
        //_file.seekg(cur_pos);
        DLog("CFileWritter:: value has been writed [%i bytes] to [%p]", sizeof(T), &_file);
    }
};






class CFileWritter {
protected:
    std::ofstream _file;

public:
    explicit CFileWritter(const bfs::path& path, bool append = false);
    ~CFileWritter();

    template<typename T>
    void Write(const std::vector<T>& data) {
        DLog("CFileWritter:: writing vector [%i bytes] to [%p]", data.size() * sizeof(T), &_file);
        size_t size = data.size() * sizeof(T);
        auto* ptr = reinterpret_cast<const char*>(&(data[0]));
        _file.write(ptr, size);
        DLog("CFileWritter:: vector has been writted! [%i bytes] to [%p]", data.size() * sizeof(T), &_file);
    }

    template<typename T, size_t S1, size_t S2>
    void Write(const T(&data)[S1][S2]) {
        DLog("CFileWritter:: writing array [%i elements] [%i bytes] to [%p]", S1 * S2, S1 * S2 * sizeof(T), &_file);
        auto* ptr = reinterpret_cast<const char*>(&data[0]);
        _file.write(ptr, S1 * S2 * sizeof(T));
        DLog("CFileWritter:: array has been writted [%i bytes] to [%p]", S1 * S2 * sizeof(T), &_file);
    }

    template<typename T, size_t S>
    void Write(const T(&data)[S]) {
        DLog("CFileWritter:: writing array [%i elements] [%i bytes] to [%p]", S, S * sizeof(T), &_file);
        auto* ptr = reinterpret_cast<const char*>(&data[0]);
        _file.write(ptr, S * sizeof(T));
        DLog("CFileWritter:: array has been writted [%i bytes] to [%p]", S * sizeof(T), &_file);
    }

    template<typename T>
    void Write(const T* data, size_t size_array) {
        DLog("CFileWritter:: writing array [%i elements] [%i bytes] to [%p]", size_array, sizeof(T), size_array * sizeof(T), &_file);
        size_array *= sizeof(T);
        auto* ptr = reinterpret_cast<const char*>(data);
        _file.write(ptr, size_array);
        DLog("CFileWritter:: array has been writted [%i bytes] to [%p]", size_array * sizeof(T), &_file);
    }

    template<typename T>
    void Write(const T& value) {
        DLog("CFileWritter:: writing value [%i bytes] to [%p]", sizeof(T), &_file);
        auto* ptr = reinterpret_cast<const char*>(&value);
        _file.write(ptr, sizeof(T));
        DLog("CFileWritter:: value has been writed [%i bytes] to [%p]", sizeof(T), &_file);
    }
};


namespace zog {
    inline ConfigManager&		readConfig() { return ConfigManager::_instance(); }
}

#undef charge

#endif // ! CFG_SYSTEM