// Copyright (c) 2014 lsc
// Meeting.cpp
#include "Meeting.h"

/*------------
members of Meeting
  sponsor_      -> string
  participator_ -> string
  startDate_    -> Date
  endDate_      -> Date
  title_        -> string
------------*/

Meeting::Meeting() {
  sponsor_      = "";
  participator_ = "";
  title_        = "";
}

Meeting::Meeting(string spon, string part, Date start, Date end, string title) {
  sponsor_      = spon;
  participator_ = part;
  startDate_    = start;
  endDate_      = end;
  title_        = title;
}

// get member value
string Meeting::getSponsor() const      { return sponsor_;      }
string Meeting::getParticipator() const { return participator_; }
Date Meeting::getStartDate() const      { return startDate_;    }
Date Meeting::getEndDate() const        { return endDate_;      }
string Meeting::getTitle() const        { return title_;        }

// set member value
void Meeting::setSponsor(string spon)      { sponsor_      = spon;  }
void Meeting::setParticipator(string part) { participator_ = part;  }
void Meeting::setStartDate(Date start)     { startDate_    = start; }
void Meeting::setEndDate(Date end)         { endDate_      = end;   }
void Meeting::setTitle(string title)       { title_        = title; }

