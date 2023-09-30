/*
 * =====================================================================
 *
 *			XPXNgin
 * 
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#ifndef __XPLICIT_H__
#define __XPLICIT_H__

#include "Config.h"

#ifndef XPLICIT_WINDOWS

inline int fopen_s(FILE** fp, const char* path, const char* res) noexcept
{
    *fp = fopen(path, res);
    return errno;
}

#endif

#ifdef XPLICIT_END_OF_BUFFER
#undef XPLICIT_END_OF_BUFFER
#endif // XPLICIT_END_OF_BUFFER

#define XPLICIT_END_OF_BUFFER '\0'

XPLICIT_API void xplicit_log(const char* str);
XPLICIT_API size_t fstrlen(const char* str);
XPLICIT_API time_t xplicit_get_epoch();
XPLICIT_API FILE* xplicit_get_logger();
XPLICIT_API bool xplicit_open_logger();

#ifdef _MSC_VER
#define XPX_PACKED_STRUCT(DECL) __pragma( pack(push, 1) ) DECL; __pragma( pack(pop))
#else
#define XPX_PACKED_STRUCT(DECL) typedef DECL __attribute__((packed));
#endif

namespace XPX 
{
	using float32 = float;
	using float64 = double;

	namespace FS = std::filesystem;

    using Thread = std::thread;
	using String = std::string;

	template <typename CharType>
	using BasicString = std::basic_string<CharType>;

    using PChar = wchar_t;

	using PString = BasicString<PChar>;

	class EngineError : public std::runtime_error
	{
	public:
		EngineError(const char* error = "Unidentified Error") : std::runtime_error(error) {}
		~EngineError() = default; // let the ABI define that.

		EngineError& operator=(const EngineError&) = default;
		EngineError(const EngineError&) = default;

	};

#ifdef XPLICIT_WINDOWS
	class Win32Error : public std::runtime_error 
	{
	public:
		Win32Error(const std::string& what) : std::runtime_error(what) { mHr = GetLastError(); }
		~Win32Error() = default; // let the ABI define that.

		Win32Error& operator=(const Win32Error&) = default;
		Win32Error(const Win32Error&) = default;

		const HRESULT& hr() { return mHr; }

	private:
		HRESULT mHr;

	};

	template <typename T>
	class ModuleType;
	class Win32Helper;
	class ModuleManagerWin32;

	template <typename T>
	class ModuleType final 
	{
		ModuleType() = default;
		friend ModuleManagerWin32;

	public:
		ModuleType(T t) : m_ptr(t) {}

		T get() { return m_ptr; }
		T operator->() { return m_ptr; }

	private:
		T m_ptr;

	};

	class Win32Helper final 
	{
	public:
		static HWND find_wnd(const wchar_t* compare) 
		{
			if (!compare) return nullptr;

			HWND hCurWnd = nullptr;
			wchar_t string[256];

			do
			{
				ZeroMemory(&string, 255);

				hCurWnd = FindWindowEx(nullptr, hCurWnd, nullptr, nullptr);
				GetWindowTextW(hCurWnd, string, 256);

				if (string == compare || !lstrcmpW(compare, string))
					return hCurWnd;

			} while (hCurWnd != nullptr);

			return nullptr;
		}

	};

	class ModuleManagerWin32 final 
	{
	public:
		ModuleManagerWin32() = delete;

		explicit ModuleManagerWin32(const std::string& module_name)
			: hMod(nullptr)
		{ 
			assert(!module_name.empty());
			hMod = LoadLibraryExA(module_name.c_str(), nullptr, 0);

			if (!hMod) 
				throw Win32Error("LoadLibraryExA failed!");
		}

		~ModuleManagerWin32() 
		{
			if (hMod)
				FreeLibrary(hMod);
		}

		ModuleManagerWin32& operator=(const ModuleManagerWin32&) = default;
		ModuleManagerWin32(const ModuleManagerWin32&) = default;

		template <typename T>
		ModuleType<T> get(const std::string& ordinal)
		{
			if (!hMod) return {};
			if (ordinal.empty()) return {};

			T the{ reinterpret_cast<T>(GetProcAddress(hMod, ordinal.c_str())) };

			return the;
		}

	private:
		HMODULE hMod;

	};

	using ModuleManager = ModuleManagerWin32;
#endif // XPLICIT_WINDOWS

	template<typename T>
	concept IsPointer = (std::is_pointer<T>::value);

	template<typename T>
	concept NotNull = (!std::is_same<T, std::nullptr_t>::value);

	template<typename T>
	concept IsPointerAndNotNull = (IsPointer<T> && NotNull<T>);

	template <typename T>
	class Ref 
	{
	public:
		Ref() : m_ptr(nullptr) {}

		Ref(IsPointerAndNotNull auto ptr) 
			: m_ptr(ptr) 
		{
			m_ref_cnt = new int32_t(0);
			assert(m_ref_cnt);

			this->_inc_ref();
		}

		~Ref()
		{
			if (m_ref_cnt && *m_ref_cnt == 0)
			{
				if (m_ptr)
				{
					delete m_ptr;
				}
			}
		}

		int32_t count() { return *m_ref_cnt; }

		T get() { return m_ptr; }
		T operator->() { return m_ptr; }

	public:
		Ref& operator=(const Ref& ref)
		{
			m_ptr = ref.m_ptr;
			this->m_ref_cnt = ref.m_ref_cnt;

			return *this;
		}

		Ref(const Ref& ref)
		{
			m_ptr = ref.m_ptr;
			this->m_ref_cnt = ref.m_ref_cnt;
		}

		operator bool() { return m_ptr; }

		void _inc_ref() 
		{
			if (m_ref_cnt)
				++*(m_ref_cnt); 
		}

	private:
		T m_ptr;
		int32_t* m_ref_cnt{ nullptr };

	};

	template <typename T>
	static inline Ref<T> make_ref(Ref<T> cls)
	{
		Ref<T> ref{ cls };
		ref._inc_ref();

		return ref;
	}

	template <typename T>
	static inline Ref<T> make_ref(T cls)
	{
		Ref<T> ref{ cls };

		return ref;
	}

#ifdef XPLICIT_WINDOWS
	using PrivShellData = HWND;

	constexpr const char* SHELL_MANAGER_EXEC_OPEN = "open";

	class ShellManager 
	{
	public:
		static Ref<HINSTANCE> open(std::string& appname, PrivShellData priv) 
		{
			if (appname.empty()) 
				return {};

			Ref<HINSTANCE> instance = ShellExecuteA(priv, SHELL_MANAGER_EXEC_OPEN, appname.c_str(), nullptr, nullptr, SW_SHOW);
			if (!instance) throw Win32Error("HINSTANCE Error, check hr()");

			return instance;
		}

		static Ref<HINSTANCE> open(const char* appname, PrivShellData priv) 
		{
			if (!appname) 
				return {};

			Ref<HINSTANCE> instance = ShellExecuteA(priv, SHELL_MANAGER_EXEC_OPEN, appname, nullptr, nullptr, SW_SHOW);
			if (!instance) throw Win32Error("HINSTANCE Error, check hr()");

			return instance;
		}

	};
#endif

	inline std::string get_at_current_path(const std::string& program_name) 
	{
		auto path = std::filesystem::current_path();
		path /= program_name;

		try 
		{
			std::ifstream io(path);

			// if it aint exist, throw and error
			if (!io.is_open())
				throw std::bad_alloc();

		}
		catch (std::bad_alloc err) 
		{
			return "";
		}

		return path.string() + program_name;
	}

#ifdef XPLICIT_WINDOWS
	inline void fini_winsock() noexcept
	{
		WSACleanup();
	}

	inline bool init_winsock(WSADATA* dat) noexcept
	{
		if (WSAStartup(MAKEWORD(2, 2), dat) == 0)
		{
			std::atexit(fini_winsock);
			return true;
		}

		return false;
	}

	// for debug builds, also new the in-game console :p
	inline void open_terminal(FILE* fp = stdout)
	{
		AllocConsole();
		(void)freopen("CONOUT$", "w", fp);
	}
#endif

	class DialogHelper
	{
	public:
#ifdef XPLICIT_WINDOWS
        static int32_t message_box(wchar_t* title, wchar_t* message, int flags = 0)
		{
			if (!title)
				return -1;

			if (!message)
				return -1;

			return MessageBoxExW(nullptr, message, title, flags, LANG_ENGLISH);
		}
#endif // !XPLICIT_WINDOWS

#ifdef XPLICIT_WINDOWS
        static int32_t message_box(PCWSTR title, PCWSTR header, PCWSTR message, PCWSTR icon, int buttonFlags)
		{
			if (!title)
				return -1;

			if (!header)
				return -1;

			if (!message)
				return -1;

			TASKDIALOGCONFIG config = { sizeof(TASKDIALOGCONFIG) };
			int32_t clicked_button = 0;
		
			config.pszContent = message;
			config.pszMainInstruction = header;
			config.pszWindowTitle = title;
			config.pszMainIcon = icon;
			config.dwCommonButtons = TDCBF_YES_BUTTON;
			config.hwndParent = nullptr;

			TaskDialogIndirect(&config, &clicked_button, nullptr, nullptr);

			return clicked_button;
		}
#endif // !XPLICIT_WINDOWS

	};

    class XPLICIT_API Logger final
	{
	public:
		Logger() = default;
		~Logger() = default;

		Logger& operator=(const Logger&) = default;
		Logger(const Logger&) = default;

	public:
		static Logger& get_singleton() noexcept
		{
			static Logger logging;
			return logging;
		}

		auto get()
		{
			static spdlog::logger* LOGGER = nullptr;

			if (!LOGGER)
            {
#ifdef _WIN32
                auto info = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
                auto info = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

				info->set_level(spdlog::level::info);
				info->set_pattern("[%^Ngine%$] %v");

#ifdef _WIN32
                auto critical = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
                auto critical = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

                critical->set_level(spdlog::level::critical);
                critical->set_pattern("[%^Ngine%$] %v");

#ifdef _WIN32
                auto err = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
                auto err = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

				err->set_level(spdlog::level::err);
				err->set_pattern("[%^Ngine%$] %v");

				LOGGER = new spdlog::logger("EngineSink", { err, info, critical });

				std::ios_base::sync_with_stdio(false);
			}

			return LOGGER;
		}

	};

    class XPLICIT_API Timer final
	{
	public:
		Timer() noexcept : mTimer(std::chrono::steady_clock::now()) {}
		~Timer() {}

		std::chrono::steady_clock::time_point now() noexcept
		{
			return std::chrono::steady_clock::now();
		}

		const std::chrono::steady_clock::time_point& start() noexcept
		{
			return mTimer;
		}

		std::chrono::duration<double> time_since(const std::chrono::steady_clock::time_point& at) noexcept
		{
			return at - mTimer;
		}

	private:
		std::chrono::steady_clock::time_point mTimer;

	};

	class XPLICIT_API UUIDFactory final
	{
	public:
		template <short ver>
		static uuids::uuid version()
		{
			switch (ver)
			{
			case 4:
			{
				std::random_device rd;
				auto seed_data = std::array<int, std::mt19937::state_size> {};
				
				std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
				
				std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
				std::mt19937 generator(seq);

				auto gen = uuids::uuid_random_generator(generator);

				return gen();
			}
			default:
				break;
			}

			return {};
		}

	};

	class XPLICIT_API FilesystemWrapper final
	{
	public:
		FilesystemWrapper() = default;
		~FilesystemWrapper() = default;

	public:
		XPLICIT_COPY_DEFAULT(FilesystemWrapper);

	public:
		std::ofstream write(const char* outPath) const noexcept
		{
			if (std::filesystem::exists(outPath))
				return std::ofstream(outPath, std::ios::app);

			return std::ofstream(outPath);
		}

		std::ofstream open(const char* outPath) const noexcept
		{
			if (!std::filesystem::exists(outPath))
				return {};

			return std::ofstream(outPath);
		}

		bool create_directory(const char* path) const noexcept
		{
			return std::filesystem::create_directory(path);
		}

		std::filesystem::path get_temp() const noexcept
		{
			return std::filesystem::temp_directory_path();
		}

		std::filesystem::path get_engine_dir() const noexcept
		{
			String dir = XPLICIT_ENV("APPDATA");
			dir += "/XPXNgin/";

			return dir;
		}

	};

	template <typename PtrType, std::size_t Size, std::size_t Align>
	class Pool final
	{
	public:
		explicit Pool()
			:
			mIndex(0UL),
			mPointer(nullptr)
		{
			/* allocate these shits */										// type			  size
			this->mPointer = static_cast<char*>(_aligned_malloc(sizeof(char) * (sizeof(PtrType) * Size), Align));
			XPLICIT_ASSERT(mPointer); // assert that shit

			/* zero memory that shit */
			ZeroMemory(this->mPointer, sizeof(PtrType) * Size);
		}

		~Pool()
		{
			if (this->mPointer)
			{
#ifdef XPLICIT_DEBUG
				_aligned_free_dbg(this->mPointer);
#else
				_aligned_free(this->mPointer);
#endif
			}
		}

	private:
		std::size_t mIndex;
		char* mPointer;

	public:
		std::size_t size() const noexcept { return sizeof(PtrType) * Size; }
		const std::size_t& capacity() const noexcept { return mIndex; }
		char* data() const noexcept { return mPointer; }
		
	public:
		template <typename... Args>
		PtrType* allocate(Args&&... args) noexcept
		{
			if (mIndex > Size)
				return nullptr;

			PtrType* ptr = reinterpret_cast<PtrType*>(&mPointer[mIndex]);
			*ptr = PtrType(std::forward<Args>(args)...);

			mIndex += sizeof(PtrType);

			return ptr;
		}

		template <typename T>
		void deallocate(T* ptr) noexcept
		{
			if (!ptr)
				return;

			ptr->~T();
			memset(ptr, 0, sizeof(T));

			mIndex -= sizeof(PtrType);
		}

	};
	
	namespace Threading
	{
		template <typename Fn, typename... Args>
		class AsyncAction final
		{
			using FnT = Fn;

		public:
			AsyncAction() = delete;

			AsyncAction(FnT fn, Args&&... args)
			{
				XPLICIT_ASSERT(fn);
				mThread = std::thread(fn, args...);
			}

			~AsyncAction() {}

			void detach() noexcept
			{
				mThread.detach();
			}
			
			AsyncAction& operator=(const AsyncAction&) = default;
			AsyncAction(const AsyncAction&) = default;

		private:
			Thread mThread;

		};
	}

    XPLICIT_API inline BasicString<PChar> platform_string(const char* utf8)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
        return cvt.from_bytes(utf8);
    }
}

#ifdef XPLICIT_WINDOWS
#	define XPLICIT_INIT_COM XPLICIT_ASSERT(SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
#	define XPLICIT_FINI_COM CoUninitialize()
#endif // ifdef XPLICIT_WINDOWS

#define XPLICIT_CRITICAL(...) XPX::Logger::get_singleton().get()->critical(__VA_ARGS__)
#define XPLICIT_ERROR(...) XPX::Logger::get_singleton().get()->error(__VA_ARGS__)
#define XPLICIT_INFO(...) XPX::Logger::get_singleton().get()->info(__VA_ARGS__)

#endif // ifndef __XPLICIT_H__
