#!/bin/zsh

bt () {
  esc="$(echo -e "\e")"
  sed -E '/ at /{
    s# at (.*/)?([^:]+):([0-9]+)$# at '$esc'[0;33;48m\1'$esc'[1;33;48m\2'$esc'[m:'$esc'[0;33;48;2;44;44;44m\3'$esc'[m#
  }'
}

hbt () {
  esc="$(echo -e "\e")"
  source-highlight -f esc -s cpp | \
  sed -E '/ at | '$esc'\[[0-9]+mat'$esc'\[m /{
    s#'$esc'\[[0-9]+m(..)?/'$esc'\[m#/#g;
    s#'$esc'\[[0-9]+m\.'$esc'\[m#.#g;
    s# (at|'$esc'\[[0-9]+mat'$esc'\[m) (.*/)?([^:]+):'$esc'\[m'$esc'\[[0-9]+m([0-9]+)'$esc'\[m$# at '$esc'[0;33;48m\2'$esc'[1;33;48m\3'$esc'[m:'$esc'[0;33;48;2;44;44;44m\4'$esc'[m#
  }'
}

qbt () {
  grep --color=auto -C11 ' at .\+:[0-9]\+$'
}

usage () {
  echo "$0 [-hqsef] [-g gdb-arg]... [--help] command [command-args...]"
  echo ' -h,--help    show this help message and exit'
  echo ' -q           quick backtrace'
  echo ' -s           use source-highlight'
  echo ' -e           catch exception'
  echo ' -f           used as filter (... | '"$0"' -f)'
  echo ' -g           a argument for gdb'
}

filter=bt
nogdb=0
gdbargs=()
catchexcept=''

TEMP=`getopt -o hqsfeg: --long help -n "$0" -- "$@"`
[ $? -ne 0 ] && exit 1
eval set -- "$TEMP"
while true ; do
  case "$1" in
    -h|--help) usage ; exit 0 ;;
    -q) filter=qbt ; shift ;;
    -s) filter=hbt ; shift ;;
    -e) catchexcept='catch throw' ; shift ;;
    -f) nogdb=1 ; shift ;;
    -g) gdbargs += "$2" ; shift 2 ;;
    --) shift ; break ;;
    *) break ;;
  esac
done

if [ $nogdb -eq 1 ] ; then
  $filter
  exit $?
fi

[ -z "$1" ] && usage >&2 && exit 1

cmd="$1"
shift

gdb -q ${gdbargs[@]} "$cmd" <<< "$catchexcept
r ${@:q}
bt" |& $filter