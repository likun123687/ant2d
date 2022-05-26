

void LogPrintInThread(const string& str)
{
    if (Singleton<Dorothy::Application>::isDisposed() || Singleton<Dorothy::AsyncLogThread>::isDisposed() || !SharedApplication.isLogicRunning()) {
#if DORA_DEBUG
#if BX_PLATFORM_ANDROID
        __android_log_print(ANDROID_LOG_DEBUG, "dorothy debug info", "%s", str.c_str());
#else
        fmt::print("{}", str);
#endif // BX_PLATFORM_ANDROID
#endif // DORA_DEBUG
        return;
    }
    SharedApplication.invokeInLogic([str]() {
        SharedAsyncLogThread.run([str] {
#if DORA_DEBUG
#if BX_PLATFORM_ANDROID
			__android_log_print(ANDROID_LOG_DEBUG, "dorothy debug info", "%s", str.c_str());
#else
			fmt::print("{}", str);
#endif // BX_PLATFORM_ANDROID
#endif // DORA_DEBUG
			return nullptr; }, [str](Own<Values>) { LogHandler(str); });
    });
}

#if !DORA_DISABLE_ASSERT_IN_LUA
void DoraAssert(bool cond, const Slice& msg)
{
    if (Dorothy::Singleton<Dorothy::LuaEngine>::isDisposed()) {
        assert(cond);
    } else if (!cond && !SharedLuaEngine.executeAssert(cond, msg)) {
        assert(cond);
    }
}
#endif // !DORA_DISABLE_ASSERT_IN_LUA

NS_DOROTHY_END
