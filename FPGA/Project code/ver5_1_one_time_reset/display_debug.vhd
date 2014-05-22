
--------------------------------------------------------
--
--	This component shows the current value for the input 
-- counter. It multiplexes between the 4 displays with
-- 10 kHz.
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity display_debug is
		port(
				Clk:				IN  STD_LOGIC;
				Clk_1kHz:		IN  STD_LOGIC;
				ones:				IN  STD_LOGIC_VECTOR(3 downto 0);
				tens:				IN  STD_LOGIC_VECTOR(3 downto 0);
				hundreds:		IN  STD_LOGIC_VECTOR(3 downto 0);
				thousands:		IN  STD_LOGIC_VECTOR(3 downto 0);
				reset:			IN  STD_LOGIC;
				an:				OUT STD_LOGIC_VECTOR(3 downto 0);
				a_to_g:			OUT STD_LOGIC_VECTOR(6 downto 0)
				);
end display_debug;

architecture Behavioral of display_debug is
signal s: 							STD_LOGIC_VECTOR(1 downto 0);
signal sig:							STD_LOGIC_VECTOR(3 downto 0);
signal show_count:				STD_LOGIC := '0';	

begin
process(clk)
	
	constant zero: 	STD_LOGIC_VECTOR(6 downto 0):= 	"0000001";
	constant one: 		STD_LOGIC_VECTOR(6 downto 0):= 	"1001111";
	constant two: 		STD_LOGIC_VECTOR(6 downto 0):= 	"0010010";
	constant three:	STD_LOGIC_VECTOR(6 downto 0):= 	"0000110";
	constant four: 	STD_LOGIC_VECTOR(6 downto 0):= 	"1001100";
	constant five: 	STD_LOGIC_VECTOR(6 downto 0):= 	"0100100";
	constant six: 		STD_LOGIC_VECTOR(6 downto 0):= 	"0100000";
	constant seven: 	STD_LOGIC_VECTOR(6 downto 0):= 	"0001111";
	constant eight:	STD_LOGIC_VECTOR(6 downto 0):= 	"0000000";
	constant nine: 	STD_LOGIC_VECTOR(6 downto 0):= 	"0000100";
	constant illegal:	STD_LOGIC_VECTOR(6 downto 0):=	"0000000";
	variable var1:		STD_LOGIC_VECTOR(6 downto 0);
		
begin 

	if rising_edge(clk) then 
		case s is 
			when "00" =>
				an <= "1110";
				sig <= ones;
			when "01" =>
				an <= "1101";
				sig <= tens;
			when "10" =>
				an <= "1011";
				sig <= hundreds;
			when "11" =>
				an <= "0111";
				sig <= thousands;
			when others =>
				null;
		end case;
	
	
	if reset = '1' then
		show_count <= '1';
	end if;
	
	if show_count = '1' then
		case sig is 
			when "0000" =>
				var1 := zero;
			when "0001" =>
				var1 := one;
			when "0010" =>
				var1 := two;
			when "0011" =>
				var1 := three;
			when "0100" =>
				var1 := four;
			when "0101" =>
				var1 := five;
			when "0110" =>
				var1 := six;
			when "0111" =>
				var1 := seven;
			when "1000" =>
				var1 := eight;
			when "1001" =>
				var1 := nine;
			when others =>
				null;
		end case;
	else
		var1 := illegal;
	end if;
				
		a_to_g <= var1;
	end if;
end process;


process(clk_1kHz)
variable temp: STD_LOGIC_VECTOR (1 downto 0):= "00";
begin
	if rising_edge(clk_1kHz) then
		temp := temp +1;
		case temp is
			when "00" =>
				s <= "01";
			when "01" =>
				s <= "10";			
			when "10" =>
				s <= "11";			
			when "11" =>
				s <= "00";
			when others =>
				null;
		end case;
	end if;
end process;


end Behavioral;

