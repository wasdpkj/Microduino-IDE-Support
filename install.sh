#!/usr/bin/env bash

echo -e "\n\n install Version:V89 \n\n";

# we need bash 4 for associative arrays
if [ "${BASH_VERSION%%[^0-9]*}" -lt "4" ]; then
  echo "BASH VERSION < 4: ${BASH_VERSION}" >&2
  exit 1
fi

# associative array for the platforms that will be verified in build_main_platforms()
# this will be eval'd in the functions below because arrays can't be exported
export MAIN_PLATFORMS='declare -A main_platforms=( [uno]="arduino:avr:uno" [due]="arduino:sam:arduino_due_x" [zero]="arduino:samd:zero"[leonardo]="arduino:avr:leonardo" )'

# associative array for other platforms that can be called explicitly in .travis.yml configs
# this will be eval'd in the functions below because arrays can't be exported
export AUX_PLATFORMS='declare -A aux_platforms=( [core328]="microduino:avr:mddevice:cpu=16MHzatmega328" [coreplus644]="microduino:avr:mddevice:cpu=16MHzatmega644" [coreplus1284]="microduino:avr:mddevice:cpu=16MHzatmega1284" [coreusb]="microduino:avr:mddevice:cpu=usb32u416m" [corerf]="microduino:avr:mddevice:cpu=16MHzatmega128rfa1" [coreesp]="microduino:esp32:CoreESP32:FlashFreq=80" )'

# make display available for arduino CLI1.
/sbin/start-stop-daemon --start --quiet --pidfile /tmp/custom_xvfb_1.pid --make-pidfile --background --exec /usr/bin/Xvfb -- :1 -ac -screen 0 1280x1024x16
sleep 3
export DISPLAY=:1.0

pip install pyserial

# download and install arduino 1.8.10
wget https://downloads.arduino.cc/arduino-1.8.10-linux64.tar.xz
tar xf arduino-1.8.10-linux64.tar.xz
mv arduino-1.8.10 $HOME/arduino_ide

#cd $HOME/arduino_ide/hardware
#mkdir Microduino
#cd Microduino
# move this library to the arduino libraries folder
#ln -s $TRAVIS_BUILD_DIR/libraries/* $HOME/arduino_ide/libraries
rm -rf $HOME/arduino_ide/libraries
mkdir -p $HOME/arduino_ide/libraries
mv -f $TRAVIS_BUILD_DIR/libraries/* $HOME/arduino_ide/libraries
#mv -f $TRAVIS_BUILD_DIR/libraries $HOME/arduino_ide/libraries


#echo -e "\n=============================================================";
#echo -e $HOME/arduino_ide/libraries/*
#echo -e "=============================================================\n";

pushd $HOME/arduino_ide/libraries/
echo -e "\n=============================================================";
ls -d */
echo -e "=============================================================\n";
popd


# add the arduino CLI to our PATH
export PATH="$HOME/arduino_ide:$PATH"

echo -e "\n########################################################################";
echo "INSTALLING DEPENDENCIES"
echo "########################################################################";


# install the microduino board packages
echo -n "ADD PACKAGE INDEX: "
DEPENDENCY_OUTPUT=$(arduino --pref "boardsmanager.additional.urls=https://raw.githubusercontent.com/wasdpkj/Microduino-IDE-Support/master/_for_travis/package_microduino_index.json" --save-prefs 2>&1)
if [ $? -ne 0 ]; then echo -e "\xe2\x9c\x96"; else echo -e "\xe2\x9c\x93"; fi

echo -n "MICRODUINO ESP32: "
DEPENDENCY_OUTPUT=$(arduino --install-boards microduino:esp32:1.8.4+6.0 2>&1)
if [ $? -ne 0 ]; then echo -e "\xe2\x9c\x96"; else echo -e "\xe2\x9c\x93"; fi

#- ln -s $TRAVIS_BUILD_DIR esp32
#cd $HOME/travis/\.arduino15/packages/microduino/hardware/esp32/\1\.\8\.\4/tools
#python get.py
#export PATH="$HOME/arduino_ide:$TRAVIS_BUILD_DIR/\.arduino15/packages/microduino/hardware/esp32/\1\.\8\.\4/tools/xtensa-esp32-elf/bin:$PATH"
#which arduino
#cd $TRAVIS_BUILD_DIR

echo -n "MICRODUINO AVR: "
DEPENDENCY_OUTPUT=$(arduino --install-boards microduino:avr 2>&1)
if [ $? -ne 0 ]; then echo -e "\xe2\x9c\x96"; else echo -e "\xe2\x9c\x93"; fi

# install random lib so the arduino IDE grabs a new library index
# see: https://github.com/arduino/Arduino/issues/3535
#echo -n "UPDATE LIBRARY INDEX: "
#DEPENDENCY_OUTPUT=$(arduino --install-library USBHost > /dev/null 2>&1)
#if [ $? -ne 0 ]; then echo -e "\xe2\x9c\x96"; else echo -e "\xe2\x9c\x93"; fi

# set the maximal compiler warning level
echo -n "SET BUILD PREFERENCES: "
DEPENDENCY_OUTPUT=$(arduino --pref "compiler.warning_level=dafault" --save-prefs 2>&1)
if [ $? -ne 0 ]; then echo -e "\xe2\x9c\x96"; else echo -e "\xe2\x9c\x93"; fi

# init the json temp var for the current platform
export PLATFORM_JSON=""

# init test stats counters
export PASS_COUNT=0
export SKIP_COUNT=0
export FAIL_COUNT=0
export PDE_COUNT=0

# build all of the examples for the passed platform
function build_platform()
{

  # arrays can't be exported, so we have to eval
  eval $MAIN_PLATFORMS
  eval $AUX_PLATFORMS

  # reset platform json var
  PLATFORM_JSON=""

  # expects argument 1 to be the platform key
  local platform_key=$1

  # placeholder for platform
  local platform=""

  # track the exit code for this platform
  local exit_code=0

  # grab all pde and ino example sketches
  declare -a examples

  # loop through results and add them to the array
  #examples=($(find $PWD -name "*.pde" -o -name "*.ino"))
  examples=($(find $HOME/arduino_ide/libraries -name "*.ino"))

  # get the last example in the array
  local last="${examples[@]:(-1)}"
  

  # grab the platform info from array or bail if invalid
  if [[ ${main_platforms[$platform_key]} ]]; then
    platform=${main_platforms[$platform_key]}
  elif [[ ${aux_platforms[$platform_key]} ]]; then
    platform=${aux_platforms[$platform_key]}
  else
    echo "NON-STANDARD PLATFORM KEY: $platform_key"
    platform=$platform_key
  fi

  echo -e "\n########################################################################";

  echo -n "SWITCHING TO ${platform_key}: "

  # switch to the requested board.
  # we have to avoid reading the exit code of local:
  # "when declaring a local variable in a function, the local acts as a command in its own right"
  local platform_stdout
  platform_stdout=$(arduino --board $platform --save-prefs 2>&1)

  # grab the exit status of the arduino board change
  local platform_switch=$?

  # notify if the platform switch failed
  if [ $platform_switch -ne 0 ]; then
    # heavy X
    echo -e "\xe2\x9c\x96"
    echo $platform_stdout
    exit_code=1
  else
    # heavy checkmark
    echo -e "\xe2\x9c\x93"
  fi

  echo "########################################################################";

  # loop through example sketches
  for example in "${examples[@]}"; do

    # store the full path to the example's sketch directory
    local example_dir=$(dirname $example)

    # store the filename for the example without the path
    local example_file=$(basename $example)

    # is this the last example in the loop
    local last_example=0
    if [ "$last" == "$example" ]; then
      last_example=1
    fi

    echo -n -e "$example_dir \t#"
    echo -n "$example_file: "

    # continue to next example if platform switch failed
    if [ $platform_switch -ne 0 ]; then
      # heavy X
      echo -e "\xe2\x9c\x96"

      # add json
      PLATFORM_JSON="${PLATFORM_JSON}$(json_sketch 0 $example_file $last_example)"

      # increment fails
      FAIL_COUNT=$((FAIL_COUNT + 1))

      # mark fail
      exit_code=1

      continue

    fi

    # ignore this example if there is an all platform skip
    if [[ -f "${example_dir}/.test.skip" ]]; then

      # right arrow
      echo -e "\xe2\x9e\x9e"

      # add json
      PLATFORM_JSON="${PLATFORM_JSON}$(json_sketch -1 $example_file $last_example)"

      # increment skips
      SKIP_COUNT=$((SKIP_COUNT + 1))

      continue

    fi

    # ignore this example if there is a skip file preset for this specific platform
    if [[ -f "${example_dir}/.$platform_key.test.skip" ]]; then

      # right arrow
      echo -e "\xe2\x9e\x9e"

      # add json
      PLATFORM_JSON="${PLATFORM_JSON}$(json_sketch -1 $example_file $last_example)"

      # increment skips
      SKIP_COUNT=$((SKIP_COUNT + 1))

      continue
    fi

    # make sure that all examples are .ino files
    #if [[ $example =~ \.pde$ ]]; then

      # heavy X
      #echo -e "\xe2\x9c\x96"

      #echo -e "-------------------------- DEBUG OUTPUT --------------------------\n"
      #echo "PDE EXTENSION. PLEASE UPDATE TO INO"
      #echo -e "\n------------------------------------------------------------------\n"

      # add json
      #PLATFORM_JSON="${PLATFORM_JSON}$(json_sketch 0 $example_file $last_example)"

      # increment fails
      #FAIL_COUNT=$((FAIL_COUNT + 1))

      # mark as fail
      #exit_code=1

      #continue

    #fi

    # verify the example, and save stdout & stderr to a variable
    # we have to avoid reading the exit code of local:
    # "when declaring a local variable in a function, the local acts as a command in its own right"
    local build_stdout
    build_stdout=$(arduino --verify $example 2>&1)

    # echo output if the build failed
    if [ $? -ne 0 ]; then

      # heavy X
      echo -e "\xe2\x9c\x96"

      echo -e "----------------------------- DEBUG OUTPUT -----------------------------\n"
      echo "$build_stdout"
      echo -e "\n------------------------------------------------------------------------\n"

      # add json
      PLATFORM_JSON="${PLATFORM_JSON}$(json_sketch 0 $example_file $last_example)"

      # increment fails
      FAIL_COUNT=$((FAIL_COUNT + 1))

      # mark as fail
      exit_code=1

    else

      # heavy checkmark
      echo -e "\xe2\x9c\x93"

      # add json
      PLATFORM_JSON="${PLATFORM_JSON}$(json_sketch 1 "$example_file" $last_example)"

      # increment passes
      PASS_COUNT=$((PASS_COUNT + 1))

    fi

  done

  return $exit_code

}

# build all examples for every platform in $main_platforms
function build_main_platforms()
{

  # arrays can't be exported, so we have to eval
  eval $MAIN_PLATFORMS

  # track the build status all platforms
  local exit_code=0

  # var to hold platforms
  local platforms_json=""

  # get the last element in the array
  local last="${main_platforms[@]:(-1)}"

  # loop through platforms in main platforms assoc array
  for p_key in "${!main_platforms[@]}"; do

    # is this the last platform in the loop
    local last_platform=0
    if [ "$last" == "${main_platforms[$p_key]}" ]; then
      last_platform=1
    fi

    # build all examples for this platform
    build_platform $p_key

    # check if build failed
    if [ $? -ne 0 ]; then
      platforms_json="${platforms_json}$(json_platform $p_key 0 "$PLATFORM_JSON" $last_platform)"
      exit_code=1
    else
      platforms_json="${platforms_json}$(json_platform $p_key 1 "$PLATFORM_JSON" $last_platform)"
    fi

  done

  # exit code is opposite of json build status
  if [ $exit_code -eq 0 ]; then
    json_main_platforms 1 "$platforms_json"
  else
    json_main_platforms 0 "$platforms_json"
  fi

  return $exit_code

}

# generate json string for a sketch
function json_sketch()
{

  # -1: skipped, 0: failed, 1: passed
  local status_number=$1

  # the filename of the sketch
  local sketch=$2

  # is this the last sketch for this platform? 0: no, 1: yes
  local last_sketch=$3

  # echo out the json
  echo -n "\"$sketch\": $status_number"

  # echo a comma unless this is the last sketch for the platform
  if [ $last_sketch -ne 1 ]; then
    echo -n ", "
  fi

}

# generate json string for a platform
function json_platform()
{

  # the platform key from main platforms or aux platforms
  local platform_key=$1

  # 0: failed, 1: passed
  local status_number=$2

  # the json string for the verified sketches
  local sketch_json=$3

  # is this the last platform we are building? 0: no, 1: yes
  local last_platform=$4

  echo -n "\"$platform_key\": { \"status\": $status_number, \"builds\": { $sketch_json } }"

  # echo a comma unless this is the last sketch for the platform
  if [ $last_platform -ne 1 ]; then
    echo -n ", "
  fi

}

# generate final json string
function json_main_platforms()
{

  # 0: failed, 1: passed
  local status_number=$1

  # the json string for the main platforms
  local platforms_json=$2

  local repo=$(git config --get remote.origin.url)

  echo -e "\n||||||||||||||||||||||||||||| JSON STATUS ||||||||||||||||||||||||||||||"

  echo -n "{ \"repo\": \"$repo\", "
  echo -n "\"status\": $status_number, "
  echo -n "\"passed\": $PASS_COUNT, "
  echo -n "\"skipped\": $SKIP_COUNT, "
  echo -n "\"failed\": $FAIL_COUNT, "
  echo "\"platforms\": { $platforms_json } }"

  echo -e "||||||||||||||||||||||||||||| JSON STATUS ||||||||||||||||||||||||||||||\n"

}
