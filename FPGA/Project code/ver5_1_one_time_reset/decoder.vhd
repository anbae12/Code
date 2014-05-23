
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity decoder is
		port(
				Clk:					IN  STD_LOGIC;
				HallsensorA:		IN  STD_LOGIC;
				HallsensorB:		IN  STD_LOGIC;
				Reset: 				IN  STD_LOGIC;
				count_direction:	OUT STD_LOGIC;
				s_ones:				OUT STD_LOGIC_VECTOR(3 downto 0);
				s_tens:				OUT STD_LOGIC_VECTOR(3 downto 0);
				s_hundreds:			OUT STD_LOGIC_VECTOR(3 downto 0);
				s_thousands:		OUT STD_LOGIC_VECTOR(3 downto 0);
				Count:				OUT STD_LOGIC_VECTOR(10 downto 0)
				);
end decoder;

architecture Behavioral of decoder is

signal A_delayed: unsigned(2 downto 0) := "000";								-- This holds the last 3 states of encoder A on motor 1.
signal B_delayed: unsigned(2 downto 0) := "000";								-- This holds the last 3 states of encoder B on motor 1.
signal count_enable_tmp, count_direction_tmp:			STD_LOGIC;
signal count_tmp:													STD_LOGIC_VECTOR(10 downto 0):= "01111101000";														


begin

--- THIS SHOULD KEEP TRACK OF THE TICS FROM MOTOR 1 ---
process (Clk)
variable ones: 			Integer range 0 to 10:= 0;
variable tens:				Integer range 0 to 10:= 0;
variable hundreds:		Integer range 0 to 10:= 0;
variable thousands: 		Integer range 0 to 10:= 1;
begin
	if rising_edge (Clk) then
		A_Delayed <= (A_Delayed(1), A_Delayed(0), HallsensorA);
		B_Delayed <= (B_Delayed(1), B_Delayed(0), HallsensorB);
		
		if Count_Enable_tmp='1' then
			if Count_Direction_tmp = '0' then
				Count_tmp <= Count_tmp + 1;
				count_direction <= '0';
						--- ONLY FOR DEBUGGING PURPOSE ---
						if ones = 9 and tens = 9 and hundreds = 9 then
						thousands := thousands + 1;	hundreds := 0; tens := 0; ones := 0;
						elsif ones = 9 and tens = 9 then
						hundreds := hundreds + 1; tens := 0; ones := 0;
						elsif ones = 9 then
						tens := tens + 1; ones := 0;
						else 
						ones := ones + 1;
						end if;
						----------------------------------
			else
				Count_tmp <= Count_tmp - 1;
				count_direction <= '1';
				if count_tmp = "00000000000" then
					count_tmp <= "10000111000"; -- SET TO 1080
				end if;
					--- ONLY FOR DEBUGGING PURPOSE ---
					if ones = 0 and tens = 0 and hundreds = 0 and thousands = 0 then
					ones := 9 ;	tens := 7;	hundreds := 0; thousands := 1;
					elsif ones = 0 and tens = 0 and hundreds = 0 then
					thousands := thousands - 1;	hundreds := 9; tens := 9; ones := 9;
					elsif ones = 0 and tens = 0 then
					hundreds := hundreds - 1; tens := 9; ones := 9;
					elsif ones = 0 then
					tens := tens - 1; ones := 9;
					else 
					ones := ones - 1;
					end if;
					----------------------------------
				
			end if;
		end if;
		
		if Reset = '1' then
			count_tmp <= "00000000000";
			--- ONLY FOR DEBUGGING PURPOSE ---
			thousands := 0;
			hundreds := 0;
			tens := 0;
			ones := 0;
			----------------------------------
		end if;
		count <= count_tmp;
		s_ones <= conv_std_logic_vector(ones,4);
		s_tens <= conv_std_logic_vector(tens,4);
		s_hundreds <= conv_std_logic_vector(hundreds,4);
		s_thousands <= conv_std_logic_vector(thousands,4);
	end if;
end process;
 
Count_Enable_tmp <= (A_Delayed(1) xor A_Delayed(2)) xor (B_Delayed(1) xor B_Delayed(2));
Count_Direction_tmp <= A_Delayed(1) xor B_Delayed(2) when count_enable_tmp = '1';
end Behavioral;

