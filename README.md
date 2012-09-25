Ranged Number Types
===================

The RANT Library provides ranged integers and floating point types for C++11.

Over and under runs can be both, a correctness and security threat. RANT
therefore provides runtime checking of ranges. Range violations produce
exceptions which can then be handled safely.

Checks are supposed to be lightweight enough to be included in production
code. If required, ranged types can be replaced by their native counterparts
via compile flag to get rid of any overhead.


Licensing
---------
Copyright (c) 2012, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)

Distributed under the terms of the GPLv2 or newer
