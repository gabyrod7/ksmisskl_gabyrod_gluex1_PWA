#!/bin/bash

run_filter() {
	root -l -q -b "filter_tree.C(\"$tree\")"
#	mv snapshot.root $tree
}

tree=$1
run_filter
