// cmdline_forget_new.cc
//
//   Copyright 2004 Daniel Burrows

#include "cmdline_forget_new.h"

#include <aptitude.h>

#include <generic/apt/apt.h>
#include <generic/apt/config_signal.h>
#include <generic/apt/text_progress.h>

#include <apt-pkg/error.h>

#include <stdio.h>

using namespace std;

using aptitude::apt::make_text_progress;
using boost::shared_ptr;

int cmdline_forget_new(int argc, char *argv[],
		       const char *status_fname, bool simulate)
{
  _error->DumpErrors();

  // NB: perhaps we should allow forgetting the new state of just
  // a few packages?
  if(argc != 1)
    {
      fprintf(stderr, _("E: The forget-new command takes no arguments\n"));
      return -1;
    }  

  shared_ptr<OpProgress> progress = make_text_progress(false);

  apt_init(progress.get(), false, status_fname);

  if(_error->PendingError())
    {
      _error->DumpErrors();
      return -1;
    }

  // In case we aren't root.
  if(!simulate)
    apt_cache_file->GainLock();
  else
    apt_cache_file->ReleaseLock();

  if(_error->PendingError())
    {
      _error->DumpErrors();
      return -1;
    }

  if(simulate)
    printf(_("Would forget what packages are new\n"));
  else
    {
      (*apt_cache_file)->forget_new(NULL);

      (*apt_cache_file)->save_selection_list(*progress);

      if(_error->PendingError())
	{
	  _error->DumpErrors();

	  return -1;
	}
    }

  return 0;
}

