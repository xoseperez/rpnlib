# RPNlib change log

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.3.0] 2019-05-24
### Added
- Added abs operator

### Changed
- Change compare operators to eq, ne, gt, ge, lt, le

## [0.2.0] 2019-05-24
### Added
- rpn_process will (optionally) fail if variable does not exist

### Fix
- rpn_variable_set will replace value if variable already exists

## [0.1.0] 2019-05-23
### Fixed
- Clear context and memory leak fix
- 
### Added
- Added more comparison operators (cmp, cmp3, index, map, constrain)
- Added casting methods (round, ceil, floor)
- AUnit testing
- Keywords file

### Changed
- Do not compile with advanced math support by default
- Renamed 'if' to 'ifn' (numeric if)

## [0.0.2] 2018-12-24
### Added
- Remote testing using Travis and PIO Plus
- Examples and more commmands
  
### Changed
- Renamed 'rpn_begin' to 'rpn_init'
- Variables now start with $ sign

## [0.0.1]
Initial version