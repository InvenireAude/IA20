#!/bin/bash
##
## File: build_all.sh
##
## Copyright (C) 2015-2020, Albert Krzymowski
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
## http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
#########################################################################

script_dir=$(dirname $(readlink -f ${0}))

platforms=$(cd $script_dir/..; echo Makefile.* | sed s/Makefile.//g )
make_targets='clean dm links all install uninstall'

make_j=-j1

function help(){
echo >&2
echo "Usage: ">&2
echo $(basename ${0}) [options] maketargets... >&2
echo "Options: ">&2
echo "  -h               This help.">&2
echo "  -d <target_dir>  Installation directory.">&2
echo "  -p <platform>    Target platform name.">&2
echo "  -f <projectlist> File containing ordered project names.">&2
echo "  -j               Enable parallel compilation.">&2
echo Available platforms: $platforms>&2
exit
}

projects_file=$script_dir/../projects.txt

while getopts ":d:p:f:j" opt; do
  case $opt in
    p)
      export IA20_PLATFORM=$OPTARG
      ;;
    d)
      export IA20_DEST_DIR=$OPTARG
      ;;
    f)
      projects_file=$OPTARG
      ;;
    j)
      make_j=-j8
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      help
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      help
      ;;
  esac
done


if [[ ! -f $projects_file ]] ; then
  echo >&2
  echo "Cannot read the list of projects from: " $projects_file >&2
  help
fi

shift $(( OPTIND - 1 ));

if [[ ! $platforms =~ (^|[[:space:]])"$IA20_PLATFORM"($|[[:space:]]) ]] ; then
  echo >&2
  echo "Set the correct target platform (with the -p option) and retry." >&2
  help
fi

echo
echo IA20 Builder welcomes.
echo
echo Configuration:
echo IA20_PLATFORM=$IA20_PLATFORM
if [ -z $IA20_DEST_DIR ]
then
echo IA20_DEST_DIR not set, make will default to /usr/local
else
echo IA20_DEST_DIR=$IA20_DEST_DIR
fi

cd ${script_dir}/../..


for target in ${*}
do
if [[ ! $make_targets =~ (^|[[:space:]])"$target"($|[[:space:]]) ]] ; then
  echo >&2
  echo "Set the correct make target." >&2
  help
fi
done

cat $projects_file | while read f
do
echo '************************************************************************'
echo '****' Project: $f
echo '************************************************************************'

if [ -d $f ]
 then
  for target in ${*}
   do
    echo Make target: $target
	(cd $f; make  ${make_j} $target)
	echo 'All done for:' $target
	echo '************************************************************************'
   done
  echo
else
 echo The source is missing, skipping $f directory.
fi

done

echo
echo All done !!!
echo


