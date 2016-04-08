#ifndef _SDHCLOGGING_H_
#define _SDHCLOGGING_H_ 1

//
// Copyright (C) Microsoft. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//
// Defining control guids, including this is required to happen before
// including the tmh file (if the WppRecorder API is used)
//
#include <WppRecorder.h>

_IRQL_requires_same_ _IRQL_requires_(PASSIVE_LEVEL)
void SDHC_LOG_INIT (
    _In_ DRIVER_OBJECT* DriverObjectPtr,
    _In_ UNICODE_STRING* RegistryPathPtr
    );

_IRQL_requires_same_ _IRQL_requires_(PASSIVE_LEVEL)
void SDHC_LOG_CLEANUP ();

//
// Helpers used by tracing macros.
// NOTE: These are not intended to be called from anywhere else
//
extern RECORDER_LOG _SdhcLogTraceRecorder;
extern int _SdhcLogBugcheck (ULONG Level);
extern int _SdhcLogDebug (ULONG Level);

//
// Tracing GUID - bf0ccf90-78ce-4f23-afd7-88dd20629c99
//
#define WPP_CONTROL_GUIDS \
    WPP_DEFINE_CONTROL_GUID(RPISDHC, (bf0ccf90,78ce,4f23,afd7,88dd20629c99), \
        WPP_DEFINE_BIT(SDHC_TRACING_DEFAULT) \
        WPP_DEFINE_BIT(SDHC_TRACING_VERBOSE) \
        WPP_DEFINE_BIT(SDHC_TRACING_DEBUG) \
        WPP_DEFINE_BIT(SDHC_TRACING_BUGCHECK) \
    )

// begin_wpp config
//
// FUNC SDHC_LOG_CRITICAL_ERROR{LEVEL=TRACE_LEVEL_CRITICAL, FLAGS=SDHC_TRACING_BUGCHECK}(MSG, ...);
// USEPREFIX (SDHC_LOG_CRITICAL_ERROR, "%!STDPREFIX! CRITICAL ERROR:");
//
// FUNC SDHC_LOG_ASSERTION{LEVEL=TRACE_LEVEL_ERROR, FLAGS=SDHC_TRACING_DEBUG}(MSG, ...);
// USEPREFIX (SDHC_LOG_ASSERTION, "%!STDPREFIX! ASSERTION :");
//
// FUNC SDHC_LOG_ERROR{LEVEL=TRACE_LEVEL_ERROR, FLAGS=SDHC_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (SDHC_LOG_ERROR, "%!STDPREFIX! ERROR :");
//
// FUNC SDHC_LOG_LOW_MEMORY{LEVEL=TRACE_LEVEL_ERROR, FLAGS=SDHC_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (SDHC_LOG_LOW_MEMORY, "%!STDPREFIX! LOW MEMORY :");
//
// FUNC SDHC_LOG_WARNING{LEVEL=TRACE_LEVEL_WARNING, FLAGS=SDHC_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (SDHC_LOG_WARNING, "%!STDPREFIX! WARNING :");
//
// FUNC SDHC_LOG_INFORMATION{LEVEL=TRACE_LEVEL_INFORMATION, FLAGS=SDHC_TRACING_DEFAULT}(MSG, ...);
// USEPREFIX (SDHC_LOG_INFORMATION, "%!STDPREFIX! INFO :");
//
// FUNC SDHC_LOG_TRACE{LEVEL=TRACE_LEVEL_VERBOSE, FLAGS=SDHC_TRACING_VERBOSE}(MSG, ...);
// USEPREFIX (SDHC_LOG_TRACE, "%!STDPREFIX! TRACE :");
//
// FUNC SDHC_CRITICAL_ASSERT{LEVEL=TRACE_LEVEL_CRITICAL, FLAGS=SDHC_TRACING_BUGCHECK}(SDHC_CRIT_ASSERT_EXP);
// USEPREFIX (SDHC_CRITICAL_ASSERT, "%!STDPREFIX! CRITICAL ASSERTION :%s", #SDHC_CRIT_ASSERT_EXP);
//
// FUNC SDHC_ASSERT{LEVEL=TRACE_LEVEL_ERROR, FLAGS=SDHC_TRACING_DEBUG}(SDHC_ASSERT_EXP);
// USEPREFIX (SDHC_ASSERT, "%!STDPREFIX! ASSERTION :%s",  #SDHC_ASSERT_EXP);
//
// end_wpp


//
// SDHC_LOG... customization
//

#define WPP_LEVEL_FLAGS_POST(LEVEL,FLAGS) \
    ,(((WPP_BIT_ ## FLAGS) == WPP_BIT_SDHC_TRACING_BUGCHECK) ? \
            _SdhcLogBugcheck(LEVEL) : \
            (((WPP_BIT_ ## FLAGS) == WPP_BIT_SDHC_TRACING_DEBUG) ? \
                _SdhcLogDebug(LEVEL) : 1))

//
// SDHC_CRTITICAL_ASSERT customization
//

#define WPP_RECORDER_LEVEL_FLAGS_SDHC_CRIT_ASSERT_EXP_FILTER(LEVEL, FLAGS, SDHC_CRIT_ASSERT_EXP) \
    (!(SDHC_CRIT_ASSERT_EXP))

#define WPP_RECORDER_LEVEL_FLAGS_SDHC_CRIT_ASSERT_EXP_ARGS(LEVEL, FLAGS, SDHC_CRIT_ASSERT_EXP) \
    WPP_CONTROL(WPP_BIT_ ## FLAGS).AutoLogContext, LEVEL, WPP_BIT_ ## FLAGS

#define WPP_LEVEL_FLAGS_SDHC_CRIT_ASSERT_EXP_POST(LEVEL, FLAGS, SDHC_CRIT_ASSERT_EXP) \
    ,((!(SDHC_CRIT_ASSERT_EXP)) ? _SdhcLogBugcheck(LEVEL) : 1)

//
// SDHC_ASSERT customization
//

#define WPP_RECORDER_LEVEL_FLAGS_SDHC_ASSERT_EXP_FILTER(LEVEL, FLAGS, SDHC_ASSERT_EXP) \
    (!(SDHC_ASSERT_EXP))

#define WPP_RECORDER_LEVEL_FLAGS_SDHC_ASSERT_EXP_ARGS(LEVEL, FLAGS, SDHC_ASSERT_EXP) \
    WPP_CONTROL(WPP_BIT_ ## FLAGS).AutoLogContext, LEVEL, WPP_BIT_ ## FLAGS

#define WPP_LEVEL_FLAGS_SDHC_ASSERT_EXP_POST(LEVEL, FLAGS, SDHC_ASSERT_EXP) \
    ,((!(SDHC_ASSERT_EXP)) ? _SdhcLogDebug(LEVEL) : 1)


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // _SDHCLOGGING_H_
