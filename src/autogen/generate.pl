#!/usr/bin/perl

#* generate.pl
#*
#* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
#* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
#*
#* This file is part of the ARL OpenSHMEM Reference Implementation software
#* package. For license information, see the LICENSE file in the top level
#* directory of the distribution.
#*


$file_api_hdr = "shmem_api.h";
$file_inlined_hdr = "shmem_inlined.h";

system "echo \"/* $file_api_hdr\n *\n * This source file is autogenerated.  Do not edit.\n */\" > $file_api_hdr";

system "echo \"include(shmem_api_top.m4)\" | m4 - | cat -s - >> $file_api_hdr";


system "echo \"/* $file_inlined_hdr\n *\n * This source file is autogenerated.  Do not edit.\n */\" > $file_inlined_hdr";

system "echo \"include(shmem_api_top.m4)\" | m4 - | cat -s - >> $file_inlined_hdr";

####################################################

@types = (
	["32","int32_t"],
	["64","int64_t"]
);

foreach $call ("alltoall", "alltoalls", "broadcast", "collect", "fcollect") {
for($i=0; $i<=$#types; $i=$i+1) {
	$fmacro = "shmem_" . $call . "_x";
	$fargs = "\\`$types[$i][0],$types[$i][1]'";

	$gmacro = "proto_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fname = "shmem_" . $call . $types[$i][0] . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
}
}

#######################################################

@types = (
	["int","int"],
	["int32","int32_t"],
	["int64","int64_t"],
	["long","long"],
	["longlong","long long"],
	["uint","unsigned int"],
	["uint32","uint32_t"],
	["uint64","uint64_t"],
	["ulong","unsigned long"],
	["ulonglong","unsigned long long"],
	["size","size_t"],
	["ptrdiff","ptrdiff_t"]
);

foreach $call ("atomic_compare_swap", "atomic_fetch_inc", "atomic_inc") {
for($i=0; $i<=$#types; $i=$i+1) {
	$fmacro = "shmem_x_" . $call;
	$fargs = "\\`$types[$i][0],$types[$i][1]'";

	$gmacro = "proto_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fname = "shmem_" . $types[$i][0] . "_" . $call . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
}
}

##########################################################

@ops = (
	["and","__and"],
	["or","__or"],
	["xor","__xor"],
	["add","__sum"]
);

foreach $call ("atomic", "atomic_fetch") {
for($i=0; $i<=$#types; $i=$i+1) {
for($j=0; $j<=$#ops; $j=$j+1) {
	$fmacro = "shmem_x_" . $call . "_op";
	$fargs = "\\`$types[$i][0],$types[$i][1],$ops[$j][0],$ops[$j][1]'";

	$gmacro = "proto_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fname = "shmem_" . $types[$i][0] . "_" . $call . "_" . $ops[$j][0] . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
}
}
}

##########################################################

@types = (
	["int","int"],
	["int32","int32_t"],
	["int64","int64_t"],
	["long","long"],
	["longlong","long long"],
	["uint","unsigned int"],
	["uint32","uint32_t"],
	["uint64","uint64_t"],
	["ulong","unsigned long"],
	["ulonglong","unsigned long long"],
	["size","size_t"],
	["ptrdiff","ptrdiff_t"],
	["float","float"],
	["double","double"]
);

foreach $call ("atomic_fetch", "atomic_set", "atomic_swap") {
for($i=0; $i<=$#types; $i=$i+1) {
	$fmacro = "shmem_x_" . $call;
	$fargs = "\\`$types[$i][0],$types[$i][1]'";

	$gmacro = "proto_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fname = "shmem_" . $types[$i][0] . "_" . $call . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
}
}

##########################################################

@types = (
	["8","int8_t"],
	["16","int16_t"],
	["32","int32_t"],
	["64","int64_t"],
	["128","__int128_t"]
);

foreach $call ("get", "put") {
for($i=0; $i<=$#types; $i=$i+1) {
	$fargs = "\\`$types[$i][0],$types[$i][1]'";

	$gmacro = "proto_h";
	$fmacro = "shmem_" . $call . "_x";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";
	$fmacro = "shmem_" . $call . "_x_nbi";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";
	$fmacro = "shmem_i" . $call . "_x";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	$fmacro = "shmem_" . $call . "_x";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";
	$fmacro = "shmem_" . $call . "_x_nbi";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";
	$fmacro = "shmem_i" . $call . "_x";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fmacro = "shmem_" . $call . "_x";
	$fname = "shmem_" .$call . "_" . $types[$i][0] . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";

	$fmacro = "shmem_" . $call . "_x_nbi";
	$fname = "shmem_" .$call . "_" . $types[$i][0] . "_nbi.c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";

	$fmacro = "shmem_i" . $call . "_x";
	$fname = "shmem_i" .$call . "_" . $types[$i][0] . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
}
}

#############################################################

@types = (
	["short","short","I"],
	["int","int","I"],
	["long","long","I"],
	["longlong","long long","I"],
	["float","float","F"],
	["double","double","F"],
	["longdouble","long double","F"],
	["complexf","float _Complex","C"],
	["complexd","double _Complex","C"]
);

@ops = (
	["and","__and","I"],
	["or","__or","I"],
	["xor","__xor","I"],
	["sum","__sum","IFC"],
	["prod","__prod","IFC"],
	["max","__max","IF"],
	["min","__min","IF"]
);

for($i=0; $i<=$#types; $i=$i+1) {
for($j=0; $j<=$#ops; $j=$j+1) {

	if (index($ops[$j][2],$types[$i][2]) != -1) {
	$fmacro = "shmem_x_op_to_all";
	$fargs = "\\`$types[$i][0],$types[$i][1],$ops[$j][0],$ops[$j][1]'";

	$gmacro = "proto_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fname = "shmem_" . $types[$i][0] . "_" . $ops[$j][0] . "_to_all.c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
	}
}
}

############################################################

@types = (
	["short","short"],
	["int","int"],
	["long","long"],
	["longlong","long long"],
	["ushort","unsigned short"],
	["uint","unsigned int"],
	["ulong","unsigned long"],
	["ulonglong","unsigned long long"]
);


foreach $call ("test", "wait", "wait_until") {
for($i=0; $i<=$#types; $i=$i+1) {
	$fmacro = "shmem_x_" . $call;
	$fargs = "\\`$types[$i][0],$types[$i][1]'";

	$gmacro = "proto_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_api_hdr";

	$gmacro = "func_h";
	system "echo \"include($gmacro.m4)\n$gmacro($fmacro,$fargs)\" | m4 - | cat -s - >> $file_inlined_hdr";

	$gmacro = "func_c";
	$fname = "shmem_" . $types[$i][0] . "_" . $call . ".c";
	system "echo \"include($gmacro.m4)\n$gmacro($fname,$fmacro,$fargs)\" | m4 - | cat -s - > $fname";
}
}

############################################################

#open(FP,">>".$file_api_hdr);
#printf FP "#endif\n";
#close(FP);
system "echo \"include(shmem_api_bottom.m4)\" | m4 - | cat -s - >> $file_api_hdr";


#open(FP,">>".$file_inlined_hdr);
#printf FP "#endif\n";
#close(FP);
system "echo \"include(shmem_api_bottom.m4)\" | m4 - | cat -s - >> $file_inlined_hdr";

