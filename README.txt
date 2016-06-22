Welcome to the Elcano autonomous tricycle project!
Please see:
http://www.elcanoproject.org/

To get started, fork and clone the repository:
https://github.com/elcano/elcano

Parameters and settings for each trike are defined in:
libraries/Settings/Settings.h

The repository includes examples in the file:
libraries/Settings/SettingsTemplate.h

Only libraries/Settings/SettingsTemplate.h should be committed in the
repository -- the libraries/Settings/Settings.h file is site-specific.

-- Copy
   libraries/Settings/SettingsTemplate.h
   to
   libraries/Settings/Settings.h
-- Give your trike a different VEHICLE_NUMBER value.
-- Add your own trike's settings, following the examples.
-- Put your settings in a conditional for your new VEHICLE_NUMBER value.

When you pull in updates from the main repository, look for changes in
SettingsTemplate.h and if needed merge them with your own Settings.h.
Especially watch for new trike parameters used in sketches.

Sketches that need these settings should
#include <Settings.h>
*after* all other includes or definitions that are used in Settings.h.