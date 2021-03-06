//=========================================================================
// Copyright (C) 2018 The C++ Component Model(CCM) Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __CCM_UTIL_CALENDAR_GREGORIAN_H__
#define __CCM_UTIL_CALENDAR_GREGORIAN_H__

#include "ccm/util/calendar/BaseCalendar.h"
#include "ccm.util.ITimeZone.h"
#include "ccm.util.calendar.IGregorian.h"

namespace ccm {
namespace util {
namespace calendar {

class Gregorian
    : public BaseCalendar
    , public IGregorian
{
protected:
    class Date
        : public BaseCalendar::Date
    {
    public:
        ECode Constructor();

        ECode Constructor(
            /* [in] */ ITimeZone* zone);

        ECode GetNormalizedYear(
            /* [out] */ Integer* normalizedYear) override;

        ECode SetNormalizedYear(
            /* [in] */ Integer normalizedYear) override;

        ECode Clone(
            /* [in] */ const InterfaceID& iid,
            /* [out] */ IInterface** obj) override;
    };

public:
    CCM_INTERFACE_DECL();

    ECode Constructor();

    ECode GetName(
        /* [out] */ String* name) override;

    ECode GetCalendarDate(
        /* [out] */ ICalendarDate** date) override;

    ECode GetCalendarDate(
        /* [in] */ Long millis,
        /* [out] */ ICalendarDate** date) override;

    ECode GetCalendarDate(
        /* [in] */ Long millis,
        /* [in] */ ICalendarDate* date) override;

    ECode GetCalendarDate(
        /* [in] */ Long millis,
        /* [in] */ ITimeZone* zone,
        /* [out] */ ICalendarDate** date) override;

    ECode NewCalendarDate(
        /* [out] */ ICalendarDate** date) override;

    ECode NewCalendarDate(
        /* [in] */ ITimeZone* zone,
        /* [out] */ ICalendarDate** date) override;
};

}
}
}

#endif // __CCM_UTIL_CALENDAR_GREGORIAN_H__
