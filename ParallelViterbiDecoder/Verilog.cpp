#include "Verilog.h"
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>

string Verilog::folder = "";
string Verilog::quadMinimum = "";
string Verilog::viterbiDecoderTester = "";
string Verilog::viterbiDecoder = "";
string Verilog::trallisP = "";
string Verilog::trallisPTester = "";
string Verilog::hammingDistance = "";
string Verilog::correctTester = "";
string Verilog::correct = "";
string Verilog::trallisS = "";
string Verilog::trallisSTester = "";
string Verilog::minimumIndex = "";
string Verilog::matrixMultiply = "";
bool Verilog::isParallel = true;
int Verilog::inputLength = 6; //input length in bits.
int Verilog::seqLength = 2; //sequence length in bits.

static ofstream verilogFile;

Verilog::Verilog(void)
{
}


Verilog::~Verilog(void)
{
}

void Verilog::Print( int num, ... )
{
  va_list arguments;                     

  va_start ( arguments, num );           
  for ( int x = 0; x < num; x++ )        
  {
	  char* line = va_arg ( arguments, char* ); 
	  verilogFile << line;
  }
  va_end ( arguments );        
}

void Verilog::Print(string line)
{
	verilogFile << line;
}

void Verilog::Println(int num, ...)
{
	va_list arguments;                  

	va_start ( arguments, num );          
	for ( int x = 0; x < num; x++ )        
	{
	  char* line = va_arg ( arguments, char* ); 
	  verilogFile << line;
	}
	va_end ( arguments );                 
	NewLine();
}
void Verilog::Println(string line)
{
	Print(line);
	NewLine();
}
void Verilog::NewLine()
{
	Print("\n");
}
const char* S(int number)
{
	std::ostringstream ostr;
	ostr << number;
	string temp = ostr.str();
	char* result = new char[temp.length()+1];
	strcpy(result, temp.c_str()); 
	result[temp.length()] = '\0';
	return result;
}

int count_ones(string s) {
  int count = 0;

  for (int i = 0; i < s.size(); i++)
    if (s[i] == '1') count++;

  return count;
}

void Verilog::GenerateHammingDistance()
{
	verilogFile.open (hammingDistance);

	Println("`timescale 1ns / 1ps																	");
	Println("																						");
	Println("module HammingDistance(clk,item1,item2,diff);											");
	Println("	input clk;																			");
	Println(3,"	input wire [1:",S(seqLength),"] item1; 														");
	Println(3,"	input wire [1:",S(seqLength),"] item2;														");
	Println(3,"	output reg [1:",S(seqLength),"] diff;														");
	Println(3,"	wire [1:",S(seqLength),"] sum;																");
	for (int i=1; i<=seqLength; i++)
		Println(7,"	xor(sum[",S(i),"],item1[",S(i),"],item2[",S(i),"]);								");
	//Println("	xor(sum[2],item1[2],item2[2]);														");
	Println("	 																					");
	Println("	always @(clk)																		");
	Println("	begin																				");
	
	char* bin = new char[seqLength+1];
	for (int j=0; j<pow((double)2,seqLength) - 1; j++)
	{
		bin = _itoa (j, bin, 2);
		Print(3,"		if (sum === ",S(seqLength),"'b");
		for (int i=strlen(bin); i<seqLength; i++)
			Print("0");
		Println(2,bin,")");
		
		Print(3,"			diff <= ",S(seqLength),"'b");
		int ones = count_ones(bin);
		bin = _itoa (ones, bin, 2);

		for (int i=strlen(bin); i<seqLength; i++)
			Print("0");
		Println(2,bin,";");
		Println("		else 																		");
	}
	Print(3,"			diff <= ",S(seqLength),"'b");
	bin = _itoa (seqLength, bin, 2);
	for (int i=strlen(bin); i<seqLength; i++)
			Print("0");
		Println(2,bin,";");
	delete[] bin;
	
	
		
		
	/*if (seqLength == 1)
	{
		Println("		if (sum === 1'b0)															");
		Println("			diff <= 1'b0;															");
		Println("		else 																		");
		Println("			diff <= 1'b1;															");
	}
	else if (seqLength == 2)
	{
		Println("		if (sum === 2'b00)															");
		Println("			diff <= 2'b00;															");
		Println("		else if (sum === 2'b01)														");
		Println("			diff <= 2'b01;															");
		Println("		else if (sum === 2'b10)														");
		Println("			diff <= 2'b01;															");
		Println("		else																		");
		Println("			diff <= 2'b10;															");
	}*/
	Println("	end																					");
	Println("																						");
	Println("																						");
	Println("endmodule																				");





	verilogFile.close();
}

void Verilog::GenerateTrallisPTester()
{
	verilogFile.open (trallisPTester);

	Println("`timescale 1ns / 1ps																			");
	Println("																								");
	Println("module TrallisPTester;																			");
	Println("																								");
	Println("	// Inputs																					");
	Println("	reg clk;																					");
	Println(3,"	reg [",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable;											");
	Println(3,"	reg [",S(seqLength-1),":0] value;																	");
	Println("																								");
	Println("	// Outputs																					");
	Println(3,"	wire [",S(5*(seqLength*2)*(seqLength*2)-1),":0] matrix;														");
	Println("																								");
	Println("	// Instantiate the Unit Under Test (UUT)													");
	Println("	TrallisP TRP (																				");
	Println("		.clk(clk), 																				");
	Println("		.metaTable(metaTable), 																	");
	Println("		.value(value), 																			");
	Println("		.matrix(matrix)																			");
	Println("	);																							");
	Println("																								");
	Println("	initial begin																				");
	Println("		// Initialize Inputs																	");
	Println("		clk = 0;																				");
	if (seqLength == 1)
	{
		Println("		metaTable[0:0] = 1'b0; // to place 0												");
		Println("		metaTable[1:1] = 1'b1; // with 0													");
		Println("																							");
		Println("		metaTable[2:2] = 1'b1; //to place 1													");
		Println("		metaTable[3:3] = 1'b0; // with 1													");
	}
	else if (seqLength == 2)
	{
		Println("		metaTable[1:0] = 2'b00; // to place 0												");
		Println("		metaTable[3:2] = 2'b00; // with 0													");
		Println("		metaTable[5:4] = 2'b10; //to place 2												");
		Println("		metaTable[7:6] = 2'b11; // with 3													");
		Println("																							");
		Println("		metaTable[9:8] = 2'b00;																");
		Println("		metaTable[11:10] = 2'b10; // with 3													");
		Println("		metaTable[13:12] = 2'b10;															");
		Println("		metaTable[15:14] = 2'b01; // with 1													");
		Println("																							");
		Println("		metaTable[17:16] = 2'b01;															");
		Println("		metaTable[19:18] = 2'b11; // with 2													");
		Println("		metaTable[21:20] = 2'b11;															");
		Println("		metaTable[23:22] = 2'b00; // with 0													");
		Println("																							");
		Println("		metaTable[25:24] = 2'b01;															");
		Println("		metaTable[27:26] = 2'b01; // with 1													");
		Println("		metaTable[29:28] = 2'b11;															");
		Println("		metaTable[31:30] = 2'b10; // with 2													");
	}
	else if (seqLength == 3)
	{
		Println("		metaTable[2:0] = 3'b000; // to place 0												");
		Println("		metaTable[5:3] = 3'b000; // with 0													");
		Println("		metaTable[8:6] = 3'b010; //to place 2												");
		Println("		metaTable[11:9] = 3'b001; // with 1													");
		Println("		metaTable[14:12] = 3'b101; //to place 5												");
		Println("		metaTable[17:15] = 3'b010; // with 2												");
		Println("																							");
		Println("		metaTable[20:18] = 3'b000; // to place 0											");
		Println("		metaTable[23:21] = 3'b011; // with 3												");
		Println("		metaTable[26:24] = 3'b011; //to place 3												");
		Println("		metaTable[29:27] = 3'b100; // with 4												");
		Println("		metaTable[32:30] = 3'b101; //to place 5												");
		Println("		metaTable[35:33] = 3'b101; // with 5												");
		Println("																							");
		Println("		metaTable[38:36] = 3'b000; // to place 0											");
		Println("		metaTable[41:39] = 3'b110; // with 6												");
		Println("		metaTable[44:42] = 3'b011; //to place 3												");
		Println("		metaTable[47:45] = 3'b111; // with 7												");
		Println("		metaTable[50:48] = 3'b110; //to place 6												");
		Println("		metaTable[53:51] = 3'b000; // with 0												");
		Println("																							");
		Println("		metaTable[56:54] = 3'b001; // to place 1											");
		Println("		metaTable[59:57] = 3'b001; // with 1												");
		Println("		metaTable[62:60] = 3'b100; //to place 4												");
		Println("		metaTable[65:63] = 3'b010; // with 2												");
		Println("		metaTable[68:66] = 3'b110; //to place 6												");
		Println("		metaTable[71:69] = 3'b011; // with 3												");
		Println("																							");
		Println("		metaTable[74:72] = 3'b001; // to place 1											");
		Println("		metaTable[77:75] = 3'b100; // with 4												");
		Println("		metaTable[80:78] = 3'b100; //to place 4												");
		Println("		metaTable[83:81] = 3'b101; // with 5												");
		Println("		metaTable[86:84] = 3'b110; //to place 6												");
		Println("		metaTable[89:87] = 3'b110; // with 6												");
		Println("																							");
		Println("		metaTable[92:90] = 3'b001; // to place 1											");
		Println("		metaTable[95:93] = 3'b111; // with 7												");
		Println("		metaTable[98:96] = 3'b100; //to place 4												");
		Println("		metaTable[101:99] = 3'b000; // with 0												");
		Println("		metaTable[104:102] = 3'b111; //to place 7											");
		Println("		metaTable[107:105] = 3'b001; // with 1												");
		Println("																							");
		Println("		metaTable[110:108] = 3'b010; // to place 2											");
		Println("		metaTable[113:111] = 3'b010; // with 2												");
		Println("		metaTable[116:114] = 3'b100; //to place 4											");
		Println("		metaTable[119:117] = 3'b011; // with 3												");
		Println("		metaTable[122:120] = 3'b111; //to place 7											");
		Println("		metaTable[125:123] = 3'b100; // with 4												");
		Println("																							");
		Println("		metaTable[128:126] = 3'b010; // to place 2											");
		Println("		metaTable[131:129] = 3'b101; // with 5												");
		Println("		metaTable[134:132] = 3'b101; //to place 5											");
		Println("		metaTable[137:135] = 3'b110; // with 6												");
		Println("		metaTable[140:138] = 3'b111; //to place 7											");
		Println("		metaTable[143:141] = 3'b111; // with 7												");
	}




	Print(3,"		value = ",S(seqLength),"'b");
	for (int i=0; i<seqLength; i++)
		Print("1");
	Println(";");
	Println("																								");
	Println("		// Wait 100 ns for global reset to finish												");
	Println("		#100;																					");
	Println("        																						");
	Println("		// Add stimulus here																	");
	Println("																								");
	Println("	end																							");
	Println("   always begin																				");
	Println("     #10  clk = ~clk; // Toggle clock every 10 ticks											");
	Println("	end   																						");
	Println("endmodule																						");






	verilogFile.close();
}

void Verilog::GenerateTrallisP()
{
	verilogFile.open (trallisP);

	Println("`timescale 1ns / 1ps																										");
	Println("																															");
	Println(7,"module TrallisP(input clk, input wire[",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable, input wire[",S(seqLength-1),":0] value,  output reg[",S(5*pow((double)2,2*seqLength)-1),":0] matrix);	");
	Println("																															");
	Print(3,"wire[",S(seqLength*pow((double)2,seqLength)),":0] diff1");
	for (int j=2; j<=seqLength; j++)
		Print(2,", diff",S(j));
	Println(";");
	//Println(3,"wire[7:0] diff0, diff1, diff2, diff3, diff4, diff5, diff6, diff7;															");

	Print(3,"wire[",S(seqLength*pow((double)2,seqLength)),":0] to1");
	for (int j=2;j<=seqLength;j++)
		Print(2,", to",S(j));
	Println(";");
	Print(3,"wire[",S(seqLength*pow((double)2,seqLength)),":0] withValue1");
	for (int j=2;j<=seqLength;j++)
		Print(2,", withValue",S(j));
	Println(";");
	Println("integer i,row;																												");

	for (int j=0; j<seqLength;j++)
//		Println(11,"HammingDistance HD",S(i)," (.clk(clk),.item1(withValue1[",S(i*a + a-1),":",S(i*a),"]),.item2(value),.diff(diff1[",S(i*a + a-1),":",S(i*a),"]));");
		for (int i=0; i<pow((double)2,seqLength);i++)
			Println(15,"HammingDistance HD",S(i+pow((double)2,seqLength)*j)," (.clk(clk),.item1(withValue",S(j+1),"[",S(i*seqLength + seqLength-1),":",S(i*seqLength),"]),.item2(value),.diff(diff",S(j+1),"[",S(i*seqLength + seqLength-1),":",S(i*seqLength),"]));");


	/*Println("HammingDistance HD0 (.clk(clk),.item1(withValue1[1:0]),.item2(value),.diff(diff1[1:0]));									");
	Println("HammingDistance HD1 (.clk(clk),.item1(withValue1[3:2]),.item2(value),.diff(diff1[3:2]));									");
	Println("HammingDistance HD2 (.clk(clk),.item1(withValue1[5:4]),.item2(value),.diff(diff1[5:4]));									");
	Println("HammingDistance HD3 (.clk(clk),.item1(withValue1[7:6]),.item2(value),.diff(diff1[7:6]));									");
	Println("HammingDistance HD4 (.clk(clk),.item1(withValue2[1:0]),.item2(value),.diff(diff2[1:0]));									");
	Println("HammingDistance HD5 (.clk(clk),.item1(withValue2[3:2]),.item2(value),.diff(diff2[3:2]));									");
	Println("HammingDistance HD6 (.clk(clk),.item1(withValue2[5:4]),.item2(value),.diff(diff2[5:4]));									");
	Println("HammingDistance HD7 (.clk(clk),.item1(withValue2[7:6]),.item2(value),.diff(diff2[7:6]));									");*/
	Println("																															");
	Println("																															");
	for (int j=0; j<seqLength; j++)
	{
		for (int i=0; i<pow((double)2,seqLength);i++)
			Println(11,"	assign to",S(j+1),"[",S(i*seqLength+seqLength-1),":",S(i*seqLength),"] = metaTable[",S(i*2*seqLength*seqLength+seqLength*(j*2)+seqLength-1)," : ",S(i*2*seqLength*seqLength+seqLength*(j*2)),"];");
		NewLine();
	}
	/*Println("	assign to1[1:0] = metaTable[1 : 0];																						");
	Println("	assign to1[3:2] = metaTable[9 : 8]; 																					");
	Println("	assign to1[5:4] = metaTable[17 : 16];																					");
	Println("	assign to1[7:6] = metaTable[25 : 24];																					");*/
	//Println("																															");


	/*Println("	assign to2[1:0] = metaTable[5 : 4];																						");
	Println("	assign to2[3:2] = metaTable[13 : 12];																					");
	Println("	assign to2[5:4] = metaTable[21 : 20];																					");
	Println("	assign to2[7:6] = metaTable[29 : 28];																					");
	Println("																															");*/
	//NewLine();
	for (int j=0; j<seqLength; j++)
	{
		for (int i=0; i<pow((double)2,seqLength);i++)
			Println(11,"	assign withValue",S(j+1),"[",S(i*seqLength+seqLength-1),":",S(i*seqLength),"] = metaTable[",S(i*2*seqLength*seqLength+seqLength*(j*2+1)+seqLength-1)," : ",S(i*2*seqLength*seqLength+seqLength*(j*2+1)),"];");
		NewLine();
	}
	//Println("	assign withValue1[1:0] = metaTable[3 : 2];																				");
	//Println("	assign withValue1[3:2] = metaTable[11 : 10];																			");
	//Println("	assign withValue1[5:4] = metaTable[19 : 18];																			");
	//Println("	assign withValue1[7:6] = metaTable[27 : 26];																			");
	/*Println("					 																										");*/
	//NewLine();
	//for (int i=0; i<2*a;i++)
	//	Println(9,"	assign withValue2[",S(i*2+a-1),":",S(i*2),"] = metaTable[",S(i*4*a+a*(3)+a-1)," : ",S(i*4*a+a*(3)),"];");
	/*Println("	assign withValue2[1:0] = metaTable[7 : 6];																				");
	Println("	assign withValue2[3:2] = metaTable[15 : 14];																			");
	Println("	assign withValue2[5:4] = metaTable[23 : 22];																			");
	Println("	assign withValue2[7:6] = metaTable[31 : 30];																			");
	Println("																															");*/
	NewLine();
	Println("																															");
	Println("	always@(clk) 																											");
	Println("		begin  																												");
	Println(3,"			for (row = 0; row < ",S(pow((double)2,seqLength)),"; row = row + 1)																	");
	Println("				begin																										");
	for (int j=0;j<seqLength; j++)
	{
		Print(15,"					matrix[row * ",S(seqLength*10)," + to",S(j+1),"[row*",S(seqLength),"+:",S(seqLength),"] * 5 +:5] <= diff",S(j+1),"[row*",S(seqLength),"+:",S(seqLength),"] & "); 
		for (int i=0; i<5-seqLength; i++)
			Print("0");
		for (int i=0; i<seqLength; i++)
			Print("1");
		Println(";");
	}
	
	//Print(9,"					matrix[row * 20 + to2[row*",S(a),"+:",S(a),"] * 5 +:5] <= diff2[row*",S(a),"+:",S(a),"] & "); 
	//for (int i=0; i<5-a; i++)
	//	Print("0");
	//for (int i=0; i<a; i++)
	//	Print("1");
	//Println(";");
	Println("				end																											");
	Println("																															");
	Println("																															");
	Println("																															");
	Println("																															");
	Println("																															");
	Println("		end																													");
	Println("endmodule																													");






	verilogFile.close();
}

void Verilog::GenerateCorrectTester()
{
	verilogFile.open (correctTester);
	Println("`timescale 1ns / 1ps																		");
	Println("																							");
	Println("module CorrectTester;																		");
	Println("																							");
	Println("	// Inputs																				");
	Println("	reg clk;																				");
	Println(3,"	reg [",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable;																	");
	Println(3,"	reg [",S(seqLength-1),":0] fromIndex;																	");
	Println(3,"	reg [",S(10*pow((double)2,seqLength-1)-1),":0] to;																			");
	Println("																							");
	Println("	// Outputs																				");
	Println(3,"	wire [",S(seqLength-1),":0] nextIndex;																	");
	Println(3,"	wire [",S(seqLength-1),":0] rightValue;																	");
	Println("																							");
	Println("	// Instantiate the Unit Under Test (UUT)												");
	Println("	Correct uut (																			");
	Println("		.clk(clk), 																			");
	Println("		.metaTable(metaTable), 																");
	Println("		.fromIndex(fromIndex), 																");
	Println("		.to(to), 																			");
	Println("		.nextIndex(inputLengthextIndex), 																");
	Println("		.rightValue(rightValue)																");
	Println("	);																						");
	Println("																							");
	Println("	initial begin																			");
	Println("		// Initialize Inputs																");
	Println("		clk = 0;																			");
	if (seqLength == 1)
	{
		Println("		metaTable[0:0] = 1'b0; // to place 0												");
		Println("		metaTable[1:1] = 1'b1; // with 0													");
		Println("																							");
		Println("		metaTable[2:2] = 1'b1; //to place 1													");
		Println("		metaTable[3:3] = 1'b0; // with 1													");
		Println("		to[4:0] = 5'b00010;																	");
		Println("		to[9:5] = 5'bxxxxx;																	");
	}
	else if (seqLength == 2)
	{
		Println("		metaTable[1:0] = 2'b00; // to place 0												");
		Println("		metaTable[3:2] = 2'b00; // with 0													");
		Println("		metaTable[5:4] = 2'b10; //to place 2												");
		Println("		metaTable[7:6] = 2'b11; // with 3													");
		Println("																							");
		Println("		metaTable[9:8] = 2'b00;																");
		Println("		metaTable[11:10] = 2'b10; // with 3													");
		Println("		metaTable[13:12] = 2'b10;															");
		Println("		metaTable[15:14] = 2'b01; // with 1													");
		Println("																							");
		Println("		metaTable[17:16] = 2'b01;															");
		Println("		metaTable[19:18] = 2'b11; // with 2													");
		Println("		metaTable[21:20] = 2'b11;															");
		Println("		metaTable[23:22] = 2'b00; // with 0													");
		Println("																							");
		Println("		metaTable[25:24] = 2'b01;															");
		Println("		metaTable[27:26] = 2'b01; // with 1													");
		Println("		metaTable[29:28] = 2'b11;															");
		Println("		metaTable[31:30] = 2'b10; // with 2													");
		Println("		to[4:0] = 5'b00010;																	");
		Println("		to[9:5] = 5'bxxxxx;																	");
		Println("		to[14:10] = 5'b00000;																");
		Println("		to[19:15] = 5'bxxxxx;																");
	}
	else if (seqLength == 3)
	{
		Println("		metaTable[2:0] = 3'b000; // to place 0												");
		Println("		metaTable[5:3] = 3'b000; // with 0													");
		Println("		metaTable[8:6] = 3'b010; //to place 2												");
		Println("		metaTable[11:9] = 3'b001; // with 1													");
		Println("		metaTable[14:12] = 3'b101; //to place 5												");
		Println("		metaTable[17:15] = 3'b010; // with 2												");
		Println("																							");
		Println("		metaTable[20:18] = 3'b000; // to place 0											");
		Println("		metaTable[23:21] = 3'b011; // with 3												");
		Println("		metaTable[26:24] = 3'b011; //to place 3												");
		Println("		metaTable[29:27] = 3'b100; // with 4												");
		Println("		metaTable[32:30] = 3'b101; //to place 5												");
		Println("		metaTable[35:33] = 3'b101; // with 5												");
		Println("																							");
		Println("		metaTable[38:36] = 3'b000; // to place 0											");
		Println("		metaTable[41:39] = 3'b110; // with 6												");
		Println("		metaTable[44:42] = 3'b011; //to place 3												");
		Println("		metaTable[47:45] = 3'b111; // with 7												");
		Println("		metaTable[50:48] = 3'b110; //to place 6												");
		Println("		metaTable[53:51] = 3'b000; // with 0												");
		Println("																							");
		Println("		metaTable[56:54] = 3'b001; // to place 1											");
		Println("		metaTable[59:57] = 3'b001; // with 1												");
		Println("		metaTable[62:60] = 3'b100; //to place 4												");
		Println("		metaTable[65:63] = 3'b010; // with 2												");
		Println("		metaTable[68:66] = 3'b110; //to place 6												");
		Println("		metaTable[71:69] = 3'b011; // with 3												");
		Println("																							");
		Println("		metaTable[74:72] = 3'b001; // to place 1											");
		Println("		metaTable[77:75] = 3'b100; // with 4												");
		Println("		metaTable[80:78] = 3'b100; //to place 4												");
		Println("		metaTable[83:81] = 3'b101; // with 5												");
		Println("		metaTable[86:84] = 3'b110; //to place 6												");
		Println("		metaTable[89:87] = 3'b110; // with 6												");
		Println("																							");
		Println("		metaTable[92:90] = 3'b001; // to place 1											");
		Println("		metaTable[95:93] = 3'b111; // with 7												");
		Println("		metaTable[98:96] = 3'b100; //to place 4												");
		Println("		metaTable[101:99] = 3'b000; // with 0												");
		Println("		metaTable[104:102] = 3'b111; //to place 7											");
		Println("		metaTable[107:105] = 3'b001; // with 1												");
		Println("																							");
		Println("		metaTable[110:108] = 3'b010; // to place 2											");
		Println("		metaTable[113:111] = 3'b010; // with 2												");
		Println("		metaTable[116:114] = 3'b100; //to place 4											");
		Println("		metaTable[119:117] = 3'b011; // with 3												");
		Println("		metaTable[122:120] = 3'b111; //to place 7											");
		Println("		metaTable[125:123] = 3'b100; // with 4												");
		Println("																							");
		Println("		metaTable[128:126] = 3'b010; // to place 2											");
		Println("		metaTable[131:129] = 3'b101; // with 5												");
		Println("		metaTable[134:132] = 3'b101; //to place 5											");
		Println("		metaTable[137:135] = 3'b110; // with 6												");
		Println("		metaTable[140:138] = 3'b111; //to place 7											");
		Println("		metaTable[143:141] = 3'b111; // with 7												");
		Println("																							");
		Println("		to[4:0] = 5'b00010;																	");
		Println("		to[9:5] = 5'bxxxxx;																	");
		Println("		to[14:10] = 5'b00001;																");
		Println("		to[19:15] = 5'bxxxxx;																");
		Println("		to[24:20] = 5'b00010;																");
		Println("		to[29:25] = 5'bxxxxx;																");
		Println("		to[34:30] = 5'b00011;																");
		Println("		to[39:35] = 5'bxxxxx;																");
	}



	Print(3,"		fromIndex = ",S(seqLength),"'b");
	for (int i=0; i<seqLength; i++)
		Print("1");
	Println(";");
	
	Println("																							");
	Println("		// Wait 100 ns for global reset to finish											");
	Println("		#100;																				");
	Println("        																					");
	Println("		// Add stimulus here																");
	Println("																							");
	Println("	end																						");
	Println("   always begin																			");
	Println("     #10  clk = ~clk; // Toggle clock every 5 ticks										");
	Println("	end  																					");
	Println("      																						");
	Println("endmodule																					");
	

	verilogFile.close();
}

void Verilog::GenerateCorrect()
{
	verilogFile.open (correct);


	Println("`timescale 1ns / 1ps																																									");
	Println("																																														");
	Println(11,"module Correct(input clk, input wire[",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable, input wire[",S(seqLength-1),":0] fromIndex, input wire[",S(10*pow((double)2,seqLength-1)-1),":0] to, output reg[",S(seqLength-1),":0] nextIndex, output reg[",S(seqLength-1),":0] rightValue);	");		
	Println("																																														");
	Println(3,"wire[",S(seqLength-1),":0] toIndex, correctValue; 																																						");
	Println("NextIndex Next(.clk(clk), .metaTable(metaTable), .fromIndex(fromIndex), .to(to), .nextIndex(toIndex));																					");
	Println("Correction Correct(.clk(clk), .metaTable(metaTable), .toIndex(toIndex), .fromIndex(fromIndex), .rightValue(correctValue));																");
	Println("																																														");
	Println("always @(clk)																																											");
	Println("	begin																																												");
	Println("		rightValue <= correctValue;																																						");
	Println("		nextIndex <= toIndex;																																							");
	Println("	end																																													");
	Println("endmodule																																												");
	Println("																																														");


	Println(9,"module NextIndex(input clk, input wire[",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable, input wire[",S(seqLength-1),":0] fromIndex, input wire[",S(10*pow((double)2,seqLength-1)-1),":0] to, output reg[",S(seqLength-1),":0] nextIndex);		");							
	Println("																																														");
	for (int i=1; i<=seqLength; i++)
		Println(5,"reg[",S(seqLength-1),":0] toValue",S(i),";																																		");
	

	Println("integer i;																																										");
	for (int i=1; i<seqLength; i++)
		Println(3,"integer isSet",S(i),";");

	for (int i=1; i<=seqLength; i++)
		Println(3,"wire[4:0] item",S(i),";");
	
	/*for (int i=1; i<=seqLength/2-1; i++)
		Println(3,"wire[4:0] min",S(i),";");*/

	for (int i=1; i<=seqLength; i++)
		Println(5,"assign item",S(i)," = to[toValue",S(i)," * 5 +:5];									");
	/*if (seqLength == 1)
		Println("assign min1 = item1;");*/
	if (seqLength>1)
	{
		Println("wire[4:0] min1;");
		Println("Minimum M1(.item1(item1), .item2(item2), .min(min1));		");
	}
	for (int i=2; i<seqLength; i++)
	{
		Println(3,"wire[4:0] min",S(i),";");
		Println(9,"Minimum M",S(i),"(.item1(min",S(i-1),"), .item2(item",S(i+1),"), .min(min",S(i),"));		");
	}
//	Println(5,"Minimum M1(.clk(clk), .item1(item",S(seqLength/2),"), .item2(item",S(seqLength/2+1),"), .min(min));		");

	Println("always @(clk)																																											");
	Println("	begin																																												");
	for (int i=1; i<seqLength; i++)
		Println(3,"		isSet",S(i)," = 0;																																										");
	Println(3,"		for (i = 0; i < ",S(pow((double)2,seqLength)),"; i = i + 1)																																			");
	Println("		begin																																											");
	
	Print(5,"			if (metaTable[i * ",S(seqLength*seqLength*2)," +:",S(seqLength),"] === fromIndex");
	for (int i=1; i<seqLength; i++)
		Print(7,"|| metaTable[i * ",S(seqLength*seqLength*2)," + ",S(2*seqLength*i)," +:",S(seqLength),"] === fromIndex");																							
	Println(")");
	Println("			begin																																										");
	
	for (int i=1; i<seqLength; i++)
	{
		Println(3,"				if (isSet",S(i)," === 0)																																						");
		Println("				begin																																								");
		Println(3,"					toValue",S(i)," <= i;																																					");
		Println(3,"					isSet",S(i)," = 1;																																						");
		Println("				end																																									");
		Println("				else																																									");
	}
	//Println("				else																																									");
	Println(3,"					toValue",S(seqLength)," <= i;																																						");
	Println("			end																																											");
	Println("		end																																												");
	Println("																																														");
	for (int i=1; i<seqLength; i++)
	{
		Println(5,"		if (item",S(i)," === min",S(seqLength-1),")				");
		Println(3,"			nextIndex <= toValue",S(i),";		");
		Println("		else							");
		
	}
	Println(3,"			nextIndex <= toValue",S(seqLength),";		");
	Println("	end																																													");
	Println("endmodule																																												");
	Println("																																														");
	Println("																																														");
	Println(9,"module Correction(input clk, input wire[",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable, input wire[",S(seqLength-1),":0] toIndex, input wire[",S(seqLength-1),":0] fromIndex, output reg[",S(seqLength-1),":0] rightValue);												");
	Println("																																														");
	Println("always @(clk)																																											");
	Println("	begin																																												");
	Println("		case (toIndex)																																									");
	
	
	char* bin = new char[seqLength+1];
	for (int j=0; j<pow((double)2,seqLength); j++)
	{
		bin = _itoa (j, bin, 2);
		Print(3,"			",S(seqLength),"'b");
		for (int i=strlen(bin); i<seqLength; i++)
			Print("0");
		Println(2,bin,":");
//		for (int r=0; r<seqLength-1; r++)
//		{
			for (int i=0; i<seqLength-1; i++)
			{
				Println(5,"				if (fromIndex === metaTable[",S(j*(seqLength*seqLength*2)+i*(2*seqLength)+seqLength-1),":",S(j*(seqLength*seqLength*2)+i*(2*seqLength)),"])");
				Println(5,"					rightValue <= metaTable[",S(j*(seqLength*seqLength*2)+i*(2*seqLength)+1*seqLength+seqLength-1),":",S(j*(seqLength*seqLength*2)+i*(2*seqLength)+1*seqLength),"];");
				Println("				else");
			}
			Println(5,"					rightValue <= metaTable[",S(j*(seqLength*seqLength*2)+(seqLength*seqLength*2-1)),":",S(j*(seqLength*seqLength*2)+(seqLength*seqLength*2-1)-(seqLength-1)),"];");
//		}
	}
	delete[] bin;
	//if (seqLength == 1)
	//{
	//	Println("			1'b0: 																																										");
	//	Println("					rightValue <= metaTable[1:1];																																		");
	//	Println("			1'b1:																																										");
	//	Println("					rightValue <= metaTable[3:3];																																		");
	//}
	//if (seqLength == 2)
	//{
	//	Println("			2'b00: 																																										");
	//	Println("				if (fromIndex === metaTable[1:0])																																		");
	//	Println("					rightValue <= metaTable[3:2];																																		");
	//	Println("				else																																									");
	//	Println("					rightValue <= metaTable[7:6];																																		");
	//	Println("			2'b01:																																										");
	//	Println("				if (fromIndex === metaTable[9:8])																																		");
	//	Println("					rightValue <= metaTable[11:10];																																		");
	//	Println("				else																																									");
	//	Println("					rightValue <= metaTable[15:14];																																		");
	//	Println("			2'b10:																																										");
	//	Println("				if (fromIndex === metaTable[17:16])																																		");
	//	Println("					rightValue <= metaTable[19:18];																																		");
	//	Println("				else																																									");
	//	Println("					rightValue <= metaTable[23:22];																																		");
	//	Println("			2'b11:																																										");
	//	Println("				if (fromIndex === metaTable[25:24])																																		");
	//	Println("					rightValue <= metaTable[27:26];																																		");
	//	Println("				else																																									");
	//	Println("					rightValue <= metaTable[31:30];																																		");
	//}
	Println("		endcase																																											");
	Println("	end																																													");
	Println("																																														");
	Println("																																														");
	Println("endmodule																																												");
																																																


	verilogFile.close();
}

void Verilog::GenerateQuadMinimum()
{
	verilogFile.open (quadMinimum);



	Println("`timescale 1ns / 1ps																																		");
	Println("																																							");
	Println("																																							");
	Println("																																							");
	Print("module QuadMinimum(item1");
	for (int i=2; i<=pow((double)2,seqLength); i++)
		Print(2,", item",S(i));
	Println(", min);");
	

	for (int i=1; i<=pow((double)2,seqLength); i++)
		Println(3,"		input wire [4:0] item",S(i),";																													");
	Println("		output reg [4:0] min;																																"); 
	Println("																																							");
	Println("		wire[4:0] minimum;																																	");
	Println("																																							");
	
	
	
	Println("		wire[4:0] min2;																																		");
	Println("		Minimum mini1(.item1(item1),.item2(item2),.min(min2));																								");
	
	for (int i=2; i<pow((double)2,seqLength); i++)
	{
		Println(3,"		wire[4:0] min",S(i+1),";																														");
		Println(9,"		Minimum mini",S(i),"(.item1(item",S(i+1),"),.item2(min",S(i),"),.min(min",S(i+1),"));															");
	}
	NewLine();
	Println(3,"		assign minimum = min",S(pow((double)2,seqLength)),";																										");	
	
	Println("		always @(*)																																			");
	Println("		begin																																				");
	Println("			min <= minimum;																																	");
	Println("		end																																					");
	Println("																																							");
	Println("endmodule																																					");



	verilogFile.close();
}

void Verilog::GenerateViterbiDecoderTester()
{
	verilogFile.open (viterbiDecoderTester);

	Println("`timescale 1ns / 1ps																							");
	Println("																												");
	Println("																												");
	Println("module ViterbiDecoderTester;																					");
	Println("																												");
	Println("	// Inputs																									");
	Println("	reg clk;																									");
	Println(3,"	reg [",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable;																			");
	Println(3,"	reg [",S(inputLength-1),":0] wrongValues;																				");
	Println("	// Outputs																									");
	Println(3,"	wire [",S(inputLength-1),":0] correctValues;																			");
	Println("																												");
	Println("	// Instantiate the Unit Under Test (UUT)																	");
	Println("	ViterbiDecoder VD (																							");
	Println("		.clk(clk),																								");
	Println("		.metaTable(metaTable), 																					");
	Println("		.wrongValues(wrongValues), 																				");
	Println("		.correctValues(correctValues)																			");
	Println("	);																											");
	Println("																												");
	Println(5,"	reg [",S(seqLength*(seqLength*2)-1),":0] automata [",S(pow((double)2,seqLength)-1),":0];																");
	Println(3,"	reg [",S(inputLength-1),":0] values [0:0] ;																			");
	Println("	integer row, col;																							");
	Println("	initial begin																								");
	Println("		// Initialize Inputs																					");
	Println("		clk = 0;																								");
	Println("		$readmemb(\"automataInput.txt\",automata);																");
	Println("		$readmemb(\"valuesInput.txt\",values);																	");
	Println(3,"		for (row = 0; row < ",S(pow((double)2,seqLength)),"; row = row + 1)															");
	Println("		begin																									");
	Println(3,"			for (col = 0; col < ",S(2*seqLength),"; col = col + 1)																");
	Println("			begin																								");
	Println(11,"			metaTable[row * ",S(seqLength*2*seqLength)," + col*",S(seqLength)," +:",S(seqLength),"] = automata[row][col*",S(seqLength)," +:",S(seqLength),"];	");
	Println("			end																									");
	Println("		end																										");
	Println("		wrongValues = values[0];																				");
	Println("		#10;																									");
	Println(" 																												");
	Println("		// Add stimulus here																					");
	Println("																												");
	Println("	end																											");
	Println("   always begin																								");
	Println("     #10  clk = ~clk; // Toggle clock every 5 ticks															");
	Println("	end																											");
	Println("endmodule																										");


	verilogFile.close();
}

void Verilog::GenerateViterbiDecoder(int parallelism)
{
	verilogFile.open (viterbiDecoder);

	Println("`timescale 1ns / 1ps																																		");
	NewLine();
	Println(7,"module ViterbiDecoder(input clk, input wire[",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable, input wire[",S(inputLength-1),":0] wrongValues, output reg[",S(inputLength-1),":0] correctValues);		");
	NewLine();
	
	for (int i=1; i<=inputLength/seqLength; i++) 
		if (isParallel)
			Println(5,"wire [",S(5*pow((double)2,2*seqLength)-1),":0] matrix",S(i),";");
		else
			Println(5,"wire [",S(5*pow((double)2,seqLength)-1),":0] matrix",S(i),";");
		
	if (isParallel)
		for (int i=0; i<inputLength/seqLength; i++) 
			Println(9,"TrallisP TR",S(i+1)," (.clk(clk), .metaTable(metaTable), .value(wrongValues[",S(i*seqLength+(seqLength-1)),":",S(i*seqLength),"]), .matrix(matrix",S(i+1),"));");
	else
	{
		Print(5,"TrallisS TR1 (.clk(clk), .metaTable(metaTable), .value(wrongValues[",S(seqLength-1),":0]), .in(",S(5*pow((double)2,seqLength)),"'b");
		for (int i=5; i<5*pow((double)2,seqLength); i++)
			Print("x");
		Println("00000), .matrix(matrix1));");
		
		for (int i=1; i<inputLength/seqLength; i++) 
			Println(13,"TrallisS TR",S(i+1)," (.clk(clk), .metaTable(metaTable), .value(wrongValues[",S(i*seqLength+(seqLength-1)),":",S(i*seqLength),"]), .in(matrix",S(i),"[",S(5*pow((double)2,seqLength)-1),":0]), .matrix(matrix",S(i+1),"));");
	}
	
	NewLine();
		
	if (isParallel)
	{
		if (inputLength/seqLength==1)
		{
			Println(3,"wire [",S(5*pow((double)2,2*seqLength)-1),":0] product1_1;"); 
			Println("assign product1_1 = matrix1;"); 
		}
		// matrix1 * matrix2
		for (int i=1; i<=(inputLength/seqLength)/2; i++) 
		{
			Println(7,"wire [",S(5*pow((double)2,2*seqLength)-1),":0] product",S(2*i-1),"_",S(2*i),";"); 
			Println(13,"MatrixMultiply MM",S(2*i-1),"_",S(2*i)," (.clk(clk), .matrix1(matrix",S(2*i-1),"), .matrix2(matrix",S(2*i),"), .product(product",S(2*i-1),"_",S(2*i),"));");
		}
		NewLine();
		
		int p = parallelism;
		if (p > inputLength / 2)
			p = inputLength / 2;
		if (inputLength / 2 % p != 0)
		{
			p = inputLength / 2;
		}
		//cout << "By range:\n";

		for (int i = 0; i < p; i++)
		{
			int from = (inputLength / p)*i;
			int to = from + (inputLength / p);
			CreateMultiplyByRange(from + 1, to);
		}


		for (int i = 1; i < p; i++)
		{
			MultiplyProduct(1, (inputLength / p)*i, (inputLength / p)*i + 1, (inputLength / p)*(i + 1));
		}

		//cout << "\n\nOne by one:\n";
		for (int i = 0; i < p; i++)
		{
			int from = (inputLength / p)*i;
			int to = from + (inputLength / p);
			if (i == 0)
				CreateMultiplyOneByOne(2, to);
			else
				CreateMultiplyOneByOne(from, to);
		}


	}
	else
	{
		
		for (int i=1; i<=inputLength/seqLength; i++) 
		{
			Println(5,"wire [",S(5*pow((double)2,2*seqLength)-1),":0] product1_",S(i),";"); 
			Println(5,"assign product1_",S(i)," = matrix",S(i),";"); 
		}
	}
	
	NewLine();
	Print(3,"wire[",S(seqLength-1),":0] indexFrom1");
	for (int i=2; i<=inputLength/seqLength; i++) 
		Print(2,", indexFrom",S(i));
	Println(";");
	Print(3,"wire[",S(seqLength-1),":0] indexTo1");
	for (int i=2; i<=inputLength/seqLength; i++) 
		Print(2,", indexTo",S(i));
	Println(";");
	if (!isParallel)
		for (int i=2; i<=inputLength/seqLength; i++) 
			Println(5,"assign indexFrom",S(i)," = indexTo",S(i-1),";");
	NewLine();


	Println(5,"MinimumIndex MI (.clk(clk), .vector(product1_",S(inputLength/seqLength),"[",S(5*pow((double)2,seqLength)-1),":0]), .index(indexFrom1));																				");

	if (isParallel)
	{
		Print(3,"wire[",S(5*pow((double)2,seqLength)-1),":0] revertP2");
		for (int i=3; i<=inputLength/seqLength; i++) 
			Print(2,", revertP",S(i));
		Println(";");

		for (int j=2; j<inputLength/seqLength; j++)
		{
			for (int i=1; i<=pow((double)2,seqLength); i++) 
			{
				Println(19,"assign revertP",S(j),"[",S(5*i-1),":",S(5*(i-1)),"] = product1_",S(inputLength/seqLength-j+1),"[",S(5*i-1),":",S(5*(i-1)),"] + product1_",S(inputLength/seqLength-j+2),"[",S((5*pow((double)2,seqLength))*(i-1)+4),":",S((5*pow((double)2,seqLength))*(i-1)),"];															");
			}
			NewLine();
		}
		for (int i=1; i<=pow((double)2,seqLength); i++) 
		{
			Println(15,"assign revertP",S(inputLength/seqLength),"[",S(5*i-1),":",S(5*(i-1)),"] = matrix1[",S(5*i-1),":",S(5*(i-1)),"] + product1_2[",S((5*pow((double)2,seqLength))*(i-1)+4),":",S((5*pow((double)2,seqLength))*(i-1)),"];															");
		}
		NewLine();
		for (int i=2; i<=inputLength/seqLength; i++) 
			Println(9,"MinimumIndex MI",S(i)," (.clk(clk), .vector(revertP",S(i),"[",S(5*pow((double)2,seqLength)-1),":0]), .index(indexFrom",S(i),"));																				");
	}
		
	NewLine();

	Print(3,"wire[",S(seqLength-1),":0] rightValue1");
	for (int i=2; i<=inputLength/seqLength; i++) 
		Print(2,", rightValue",S(i));
	Println(";");
	NewLine();
	int remain = 0;

	for (int i=1; i<inputLength/seqLength-1; i++) 
	{
		Println(13,"Correct C",S(i)," (.clk(clk), .metaTable(metaTable), .fromIndex(indexFrom",S(i),"), .to(product1_",S(inputLength/seqLength-i),"[",S(5*pow((double)2,seqLength)-1),":0]), .nextIndex(indexTo",S(i),"), .rightValue(rightValue",S(i),"));");
		remain = i;
	}
	if (inputLength/seqLength >= 2)
	{
		remain++;
		Println(13,"Correct C",S(remain)," (.clk(clk), .metaTable(metaTable), .fromIndex(indexFrom",S(remain),"), .to(matrix",S(1),"[",S(5*pow((double)2,seqLength)-1),":0]), .nextIndex(indexTo",S(remain),"), .rightValue(rightValue",S(remain),"));");
	}
	remain++;
	Print(7,"Correct C",S(remain)," (.clk(clk), .metaTable(metaTable), .fromIndex(indexFrom",S(remain),"), .to(",S(5*pow((double)2,seqLength)),"'b");
	for (int i=5; i<5*pow((double)2,seqLength); i++)
		Print("x");
	Println(5,"00000), .nextIndex(indexTo",S(remain),"), .rightValue(rightValue",S(remain),"));");
	NewLine();
	NewLine();
	Println("	always@(clk) 																																			");
	Println("	begin  																																					");
	
	for (int i=0; i<inputLength/seqLength; i++) 
		Println(7,"		correctValues[",S(seqLength*i+(seqLength-1)),":",S(seqLength*i),"] <= rightValue",S(i+1),";");
	
	Println("	end																																						");
	Println("endmodule																																					");


	verilogFile.close();

}

void Verilog::GenerateTrallisS()
{
	verilogFile.open (trallisS);

	Println("`timescale 1ns / 1ps																																																	");
	Println("																																																						");
	Println(9,"module TrallisS(input clk, input wire[",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable, input wire[",S(seqLength-1),":0] value, input wire[",S(5*pow((double)2,seqLength)-1),":0] in,  output reg[",S(5*pow((double)2,seqLength)-1),":0] matrix);	");
	Println("																																																						");
	Println(3,"wire [",S(5*pow((double)2,2*seqLength)-1),":0] trMatrix;																																										");
	Println(3,"reg [",S(5*pow((double)2,2*seqLength)-1),":0] sumMatrix;																																										");
	Print(3,"wire[",S(seqLength*pow((double)2,seqLength)-1),":0] to1");
	for (int i=2; i<=seqLength; i++)
		Print(2,",to",S(i));
	Println(";");
	for (int i=1; i<= pow((double)2,seqLength); i++)
		Println(3,"wire [4:0] element",S(i),";																																														");
	Println("integer row;																																																			");
	Println("TrallisP TR1 (.clk(clk), .metaTable(metaTable), .value(value), .matrix(trMatrix));																																		");
	Println("																																																						");
	for (int i=0; i< pow((double)2,seqLength); i++)
	{
		Print(3,"QuadMinimum QM",S(i+1)," (");
		for (int j=0; j<pow((double)2,seqLength); j++)
		{
			if (j > 0)
				Print(", ");
			Print(5,".item",S(j+1),"(sumMatrix[",S(i*5 + j*(10*seqLength)),"+:5])");
		}
		Println(3,", .min(element",S(i+1),"));																																														");
	}
	Println("																																																						");
	for (int i=0; i<seqLength; i++)
		for (int j=0; j<pow((double)2,seqLength); j++)
			Println(11,"assign to",S(i+1),"[",S(seqLength*j+(seqLength-1)),":",S(seqLength*j),"] = metaTable[",S(j*(seqLength*seqLength*2)+i*2*seqLength+(seqLength-1))," : ",S(j*(seqLength*seqLength*2)+i*2*seqLength),"];																						");
	Println("																																																						");
	Println("																																																						");
	Println("	always@(clk) 																																																		");
	Println("		begin  	 																																																		");
	Println(3,"			for (row = 0; row < ",S(pow((double)2,seqLength)),"; row = row + 1)																																					");
	Println("					begin																																																");
	Println("						if (in[row*5+:5] !== 5'bxxxxx)																																									");
	Println("							begin																																														");
	for (int i=1;i<=seqLength; i++)
		Println(17,"								sumMatrix[row * ",S(seqLength*10)," + to",S(i),"[row*",S(seqLength),"+:",S(seqLength),"] * 5 +:5] <= trMatrix[row * ",S(seqLength*10)," + to",S(i),"[row*",S(seqLength),"+:",S(seqLength),"] * 5 +:5] + in[row*5+:5];				");
	Println("							end																																															");
	Println("					end																																																	");
	Println("																																																						");
	for (int i=0; i<pow((double)2,seqLength); i++)
		Println(5,"			matrix[",S(5*i),"+:5] <= element",S(i+1),";																																								");
	Println("																																																						");
	Println("		end																																																				");
	Println("endmodule																																																				");

	verilogFile.close();
}

void Verilog::GenerateTrallisSTester()
{
	verilogFile.open (trallisSTester);

	Println("`timescale 1ns / 1ps																			");
	Println("																								");
	Println("																								");
	Println("module TrallisSTester;																			");
	Println("																								");
	Println("	// Inputs																					");
	Println("	reg clk;																					");
	Println(3,"	reg [",S(seqLength*(seqLength*2)*pow((double)2,seqLength)-1),":0] metaTable;											");
	Println(3,"	reg [",S(seqLength-1),":0] value;																	");
	Println(3,"	reg [",S(5*pow((double)2,seqLength)-1),":0] in;														");
	Println("																								");
	Println("	// Outputs																					");
	Println(3,"	wire [",S(5*pow((double)2,seqLength)-1),":0] matrix;												");
	Println("																								");
	Println("	// Instantiate the Unit Under Test (UUT)													");
	Println("	TrallisS TRS (																				");
	Println("		.clk(clk), 																				");
	Println("		.metaTable(metaTable), 																	");
	Println("		.value(value), 																			");
	Println("		.in(in), 																				");
	Println("		.matrix(matrix)																			");
	Println("	);																							");
	Println("																								");
	Println("	initial begin																				");
	Println("		// Initialize Inputs																	");
	Println("		clk = 0;																				");
	if (seqLength == 1)
	{
		Println("		metaTable[0:0] = 1'b0; // to place 0												");
		Println("		metaTable[1:1] = 1'b1; // with 1													");
		Println("																							");
		Println("		metaTable[2:2] = 1'b1; //to place 1													");
		Println("		metaTable[3:3] = 1'b0; // with 0													");
	}
	else if (seqLength == 2)
	{
		Println("		metaTable[1:0] = 2'b00; // to place 0												");
		Println("		metaTable[3:2] = 2'b00; // with 0													");
		Println("		metaTable[5:4] = 2'b10; //to place 2												");
		Println("		metaTable[7:6] = 2'b11; // with 3													");
		Println("																							");
		Println("		metaTable[9:8] = 2'b00;																");
		Println("		metaTable[11:10] = 2'b10; // with 2													");
		Println("		metaTable[13:12] = 2'b10;															");
		Println("		metaTable[15:14] = 2'b01; // with 1													");
		Println("																							");
		Println("		metaTable[17:16] = 2'b01;															");
		Println("		metaTable[19:18] = 2'b11; // with 3													");
		Println("		metaTable[21:20] = 2'b11;															");
		Println("		metaTable[23:22] = 2'b00; // with 0													");
		Println("																							");
		Println("		metaTable[25:24] = 2'b01;															");
		Println("		metaTable[27:26] = 2'b01; // with 1													");
		Println("		metaTable[29:28] = 2'b11;															");
		Println("		metaTable[31:30] = 2'b10; // with 2													");
	}
	else if (seqLength == 3)
	{
		Println("		metaTable[2:0] = 3'b000; // to place 0												");
		Println("		metaTable[5:3] = 3'b000; // with 0													");
		Println("		metaTable[8:6] = 3'b010; //to place 2												");
		Println("		metaTable[11:9] = 3'b001; // with 1													");
		Println("		metaTable[14:12] = 3'b101; //to place 5												");
		Println("		metaTable[17:15] = 3'b010; // with 2												");
		Println("																							");
		Println("		metaTable[20:18] = 3'b000; // to place 0											");
		Println("		metaTable[23:21] = 3'b011; // with 3												");
		Println("		metaTable[26:24] = 3'b011; //to place 3												");
		Println("		metaTable[29:27] = 3'b100; // with 4												");
		Println("		metaTable[32:30] = 3'b101; //to place 5												");
		Println("		metaTable[35:33] = 3'b101; // with 5												");
		Println("																							");
		Println("		metaTable[38:36] = 3'b000; // to place 0											");
		Println("		metaTable[41:39] = 3'b110; // with 6												");
		Println("		metaTable[44:42] = 3'b011; //to place 3												");
		Println("		metaTable[47:45] = 3'b111; // with 7												");
		Println("		metaTable[50:48] = 3'b110; //to place 6												");
		Println("		metaTable[53:51] = 3'b000; // with 0												");
		Println("																							");
		Println("		metaTable[56:54] = 3'b001; // to place 1											");
		Println("		metaTable[59:57] = 3'b001; // with 1												");
		Println("		metaTable[62:60] = 3'b100; //to place 4												");
		Println("		metaTable[65:63] = 3'b010; // with 2												");
		Println("		metaTable[68:66] = 3'b110; //to place 6												");
		Println("		metaTable[71:69] = 3'b011; // with 3												");
		Println("																							");
		Println("		metaTable[74:72] = 3'b001; // to place 1											");
		Println("		metaTable[77:75] = 3'b100; // with 4												");
		Println("		metaTable[80:78] = 3'b100; //to place 4												");
		Println("		metaTable[83:81] = 3'b101; // with 5												");
		Println("		metaTable[86:84] = 3'b110; //to place 6												");
		Println("		metaTable[89:87] = 3'b110; // with 6												");
		Println("																							");
		Println("		metaTable[92:90] = 3'b001; // to place 1											");
		Println("		metaTable[95:93] = 3'b111; // with 7												");
		Println("		metaTable[98:96] = 3'b100; //to place 4												");
		Println("		metaTable[101:99] = 3'b000; // with 0												");
		Println("		metaTable[104:102] = 3'b111; //to place 7											");
		Println("		metaTable[107:105] = 3'b001; // with 1												");
		Println("																							");
		Println("		metaTable[110:108] = 3'b010; // to place 2											");
		Println("		metaTable[113:111] = 3'b010; // with 2												");
		Println("		metaTable[116:114] = 3'b100; //to place 4											");
		Println("		metaTable[119:117] = 3'b011; // with 3												");
		Println("		metaTable[122:120] = 3'b111; //to place 7											");
		Println("		metaTable[125:123] = 3'b100; // with 4												");
		Println("																							");
		Println("		metaTable[128:126] = 3'b010; // to place 2											");
		Println("		metaTable[131:129] = 3'b101; // with 5												");
		Println("		metaTable[134:132] = 3'b101; //to place 5											");
		Println("		metaTable[137:135] = 3'b110; // with 6												");
		Println("		metaTable[140:138] = 3'b111; //to place 7											");
		Println("		metaTable[143:141] = 3'b111; // with 7												");
	}
	Print(3,"		value = ",S(seqLength),"'b");
	for (int i=0; i<seqLength; i++)
		Print("1");
	Println(";");
	Print(3,"		in = ",S(5*pow((double)2,seqLength)),"'b");
	for (int i=0; i<5*pow((double)2,seqLength) - 5; i++)
		Print("x");
	Println("00000;");
	//Println("		in = 20'bxxxxxxxxxxxxxxx00000;															");
	Println("		// Wait 100 ns for global reset to finish												");
	Println("		#100;																					");
	Println("        																						");
	Println("		// Add stimulus here																	");
	Println("																								");
	Println("	end																							");
	Println("   always begin																				");
	Println("     #10  clk = ~clk; // Toggle clock every 5 ticks											");
	Println("	end   																						");
	Println("endmodule																						");




	verilogFile.close();
}

void Verilog::GenerateMinimumIndex()
{
	verilogFile.open (minimumIndex);

	Println("`timescale 1ns / 1ps																																	");
	Println("																																						");
	Println(5,"module MinimumIndex(input clk, input wire[",S(5*pow((double)2,seqLength)-1),":0] vector, output reg[",S(seqLength-1),":0] index);										");
	Println("    																																					");
	Println("wire[4:0] min;																																			");
	Println("																																						");
	Print("QuadMinimum QM(");
	for (int i=1; i<=pow((double)2,seqLength); i++)
	{
		if (i > 1)
			Print(", ");
		Print(7,".item",S(i),"(vector[",S(i*5-1),":",S((i-1)*5),"])");
	}
	Println(", .min(min));");			
	Println("																																						");
	Println("	always@(clk) 																																		");
	Println("		begin  																																			");
	Println("			if (vector[4:0] === min)																													");
	Print(3,"				index = ",S(seqLength),"'b");
	for (int i=0; i<seqLength ; i++)
		Print("0");
	Println(";");


	char* bin = new char[seqLength+1];
	for (int j=1; j<pow((double)2,seqLength) - 1; j++)
	{
		Println(5,"			else if (vector[",S((j+1)*5-1),":",S(j*5),"] === min)																					");
		bin = _itoa (j, bin, 2);
		Print(3,"				index = ",S(seqLength),"'b");
		for (int i=strlen(bin); i<seqLength; i++)
			Print("0");
		Println(2,bin,";");
	}
	Println("			else");
	Print(3,"				index = ",S(seqLength),"'b");
	for (int i=0; i<seqLength; i++)
		Print("1");
	Println(";");
	delete[] bin;
	Println("		end																																				");
	Println("endmodule																																				");


	verilogFile.close();
}

void Verilog::GenerateMatrixMultiply()
{
	verilogFile.open (matrixMultiply);

	Println("`timescale 1ns / 1ps																																												");
	Println("																																																	");
	Println(7,"module MatrixMultiply(input clk, input wire[",S(5*pow((double)2,2*seqLength)-1),":0] matrix1, input wire[",S(5*pow((double)2,2*seqLength)-1),":0] matrix2, output reg[",S(5*pow((double)2,2*seqLength)-1),":0] product);	");
	Println("																																																	");
	Print(3,"reg[",S(5*pow((double)2,2*seqLength)-1),":0] sum1");
	for (int i=2; i<=pow((double)2,seqLength); i++)
		Print(2,", sum",S(i));
	Println(";");																																								
	Println(3,"wire[",S(5*pow((double)2,2*seqLength)-1),":0] min;																																													");
	Println("																																																	");
	for (int i=1; i<=pow((double)2,2*seqLength); i++)
	{
		Print(3,"QuadMinimum QM",S(i),"(");
		for (int j=1; j<=pow((double)2,seqLength); j++)
		{
			if (j > 1)
				Print(", ");
			Print(9,".item",S(j),"(sum",S(j),"[",S(5*i-1),":",S(5*(i-1)),"])");
		}
		Println(5,", .min(min[",S(5*i-1),":",S(5*(i-1)),"]));");
	}
	Println("																																																	");
	Println("																																																	");
	Println("integer row, col;																																													");
	Println("	always@(clk) 																																													");
	Println("	begin  																																															");

	Println(3,"		for (row = 0; row < ",S(pow((double)2,seqLength)),"; row = row + 1)																																						");
	Println("		begin																																														");
	Println(3,"			for (col = 0; col < ",S(pow((double)2,seqLength)),"; col = col + 1)																																					");
	Println("			begin																																													");
	for (int i=1; i<=pow((double)2,seqLength); i++)
		Println(11,"				sum",S(i),"[row * ",S(5*pow((double)2,seqLength))," + col * 5 +:5] <= matrix1[row * ",S(5*pow((double)2,seqLength))," + ",S(5*(i-1))," +:5] + matrix2[col * 5 + ",S((i-1) * 5*pow((double)2,seqLength))," +:5];																									");
	Println("																																																	");
	Println(5,"				product[row * ",S(5*pow((double)2,seqLength))," + col * 5 +:5] <= min[row * ",S(5*pow((double)2,seqLength))," + col * 5 +:5];																														");
	Println("			end																																														");
	Println("		end																																															");
	Println("	end																																																");
	Println("endmodule																																															");


	verilogFile.close();
}

void Verilog::MultiplyMatrix(int m1_from, int m1_to, int m2)
{
	Println(7, "wire [", S(5 * pow((double)2, 2 * seqLength) - 1), ":0] product", S(m1_from), "_", S(m2), ";");
	Println(15, "MatrixMultiply MM", S(m1_from), "_", S(m2), " (.clk(clk), .matrix1(product", S(m1_from), "_", S(m1_to), "), .matrix2(matrix", S(m2), "), .product(product", S(m1_from), "_", S(m2), "));");
}

void Verilog::MultiplyProduct(int m1_from, int m1_to, int m2_from, int m2_to)
{
	Println(7, "wire [", S(5 * pow((double)2, 2 * seqLength) - 1), ":0] product", S(m1_from), "_", S(m2_to), ";");
	Println(17, "MatrixMultiply MM", S(m1_from), "_", S(m2_to), " (.clk(clk), .matrix1(product", S(m1_from), "_", S(m1_to), "), .matrix2(product", S(m2_from), "_", S(m2_to), "), .product(product", S(m1_from), "_", S(m2_to), "));");
}

void Verilog::CreateMultiplyOneByOne(int from, int to)
{
	for (int i = from; i < to - 1; i++)
	{
		if (from == 2 && (i + 1) % 2 == 0)
		{
			continue;
		}
		MultiplyMatrix(1, i, i + 1);
	}
}

void Verilog::CreateMultiplyByRange(int from, int to)
{
	for (int i = from; i < to - 2; i = i + 2)
		MultiplyProduct(from, i + 1, i + 2, i + 3);
}

