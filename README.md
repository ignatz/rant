# Ranged Number Types

The RANT Library provides ranged integers and floating point types for C++11.

Over and under runs can be both, a correctness and security threat. RANT
therefore provides runtime checking of ranges. Range violations produce
exceptions which can then be handled safely.

Checks are supposed to be lightweight enough to be included in production
code. If required, ranged types can be replaced by their native counterparts
via compile flag to get rid of any overhead.

## Licensing
Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)

rant is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

rant is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with rant.  If not, see <http://www.gnu.org/licenses/>.
