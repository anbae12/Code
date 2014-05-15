library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
 
entity QuadratureDecoder is
    Port ( QuadA 		: in  STD_LOGIC;
           QuadB 		: in  STD_LOGIC;
           Clk 		: in  STD_LOGIC;
           Position 	: out STD_LOGIC_VECTOR (7 downto 0);
			  an 			: out STD_LOGIC_VECTOR (3 downto 0);
			  a_to_g 	: out STD_LOGIC_VECTOR (6 downto 0);
			  enable		: IN STD_LOGIC;
			  reset		: IN STD_LOGIC;
			  ld			: out STD_LOGIC_VECTOR (7 downto 0));
end QuadratureDecoder;
 
architecture Behavioral of QuadratureDecoder is
 
signal QuadA_Delayed: unsigned(2 downto 0) := "000";
signal QuadB_Delayed: unsigned(2 downto 0) := "000";
 
signal Count_Enable: 															STD_LOGIC;
signal Count_Direction: 														STD_LOGIC;
signal Count: unsigned(7 downto 0) := 	"00000000";
signal s_ones, s_tens, s_hundreds, s_thousands, s_tenthousands : 	STD_LOGIC_VECTOR (3 downto 0);
signal s: 																			STD_LOGIC_VECTOR (1 downto 0);
signal clk_1kHz, clk_10Hz, clk_1Hz: 										STD_LOGIC;
signal sig:		 																	STD_LOGIC_VECTOR(3 downto 0);
signal timeout:																	STD_LOGIC;

component mux
		port(
				clk:  IN STD_LOGIC;
				s:		OUT STD_LOGIC_VECTOR (1 downto 0)
				);
end component;
	
component stop_watch
		port(
				clk_1kHz:	IN STD_LOGIC;
				enable:		IN STD_LOGIC;
				reset:		IN STD_LOGIC;
				timeout:		OUT STD_LOGIC
				);
end component;

component clock 
	port(
			clk: IN STD_LOGIC;
			clk_1kHz, clk_10Hz, clk_1Hz: OUT STD_LOGIC
			);
end component;
	
	
begin

MX:	mux port map (
		clk => clk_1kHz,
		s => s
		);
		
CL:	clock port map (
			clk => clk,
			clk_1kHz => clk_1kHz,
			clk_10Hz => clk_10Hz,
			clk_1Hz => clk_1Hz
			);
SW:	stop_watch port map (
			clk_1kHz => clk_1kHz,
			enable => enable,
			reset => reset,
			timeout => timeout
		);

process (Clk, enable)
variable ones, tens, hundreds, thousands, tenthousands: Integer range 0 to 10:= 0;
begin
	if rising_edge (Clk) then
		QuadA_Delayed <= (QuadA_Delayed(1), QuadA_Delayed(0), QuadA);
		QuadB_Delayed <= (QuadB_Delayed(1), QuadB_Delayed(0), QuadB);
		if reset = '1' then 
			ones := 0;  tens := 0; hundreds := 0; thousands := 0; tenthousands:= 0;
		end if;
		if timeout = '0' and enable = '1' then
		if Count_Enable='1' then
			if Count_Direction='1' then
				Count <= Count + 1;
				Position <= conv_std_logic_vector(Count, 8);
				if tenthousands = 9 then
					tenthousands := 1;
				end if;
				if ones = 9 and tens = 9 and hundreds = 9 and thousands = 9 then 
					ones := 0;  tens := 0; hundreds := 0; thousands := 0;
					tenthousands := tenthousands + 1;
				elsif ones = 9 and tens = 9 and hundreds = 9 then
					thousands := thousands + 1;	hundreds := 0; tens := 0; ones := 0;
				elsif ones = 9 and tens = 9 then
					hundreds := hundreds + 1; tens := 0; ones := 0;
				elsif ones = 9 then
					tens := tens + 1; ones := 0;
				else 
					ones := ones + 1;
				end if;
			else
				Count <= Count - 1;
				Position <= conv_std_logic_vector(Count, 8);
			end if;
		s_ones <= conv_std_logic_vector(ones,4);
		s_tens <= conv_std_logic_vector(tens,4);
		s_hundreds <= conv_std_logic_vector(hundreds,4);
		s_thousands <= conv_std_logic_vector(thousands,4);
		s_tenthousands <= conv_std_logic_vector(tenthousands, 4);
		end if;
		end if;
	end if;

end process;
 
Count_Enable <= QuadA_Delayed(1) xor QuadA_Delayed(2) xor QuadB_Delayed(1)
				xor QuadB_Delayed(2);
Count_Direction <= QuadA_Delayed(1) xor QuadB_Delayed(2);


process(clk)
		variable zero: 	STD_LOGIC_VECTOR(6 downto 0);
		variable one: 		STD_LOGIC_VECTOR(6 downto 0);
		variable two: 		STD_LOGIC_VECTOR(6 downto 0);
		variable three:	STD_LOGIC_VECTOR(6 downto 0);
		variable four: 	STD_LOGIC_VECTOR(6 downto 0);
		variable five: 	STD_LOGIC_VECTOR(6 downto 0);
		variable six: 		STD_LOGIC_VECTOR(6 downto 0);
		variable seven: 	STD_LOGIC_VECTOR(6 downto 0);
		variable eight:	STD_LOGIC_VECTOR(6 downto 0);
		variable nine: 	STD_LOGIC_VECTOR(6 downto 0);
		variable var1:		STD_LOGIC_VECTOR(6 downto 0);
		
	begin 
	zero 	:= 	"0000001";
	one 	:= 	"1001111";
	two 	:= 	"0010010";
	three := 	"0000110";
	four 	:= 	"1001100";
	five 	:= 	"0100100";
	six 	:= 	"0100000";
	seven := 	"0001111";
	eight := 	"0000000";
	nine 	:= 	"0000100";

	if rising_edge(clk) then 
		case s is 
			when "00" =>
				an <= "1110";
				sig <= s_ones;
			when "01" =>
				an <= "1101";
				sig <= s_tens;
			when "10" =>
				an <= "1011";
				sig <= s_hundreds;
			when "11" =>
				an <= "0111";
				sig <= s_thousands;
			when others =>
				null;
		end case;
	
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
		
		case s_tenthousands is
			when "0000" =>
				ld <= "00000000";
			when "0001" =>
				ld <= "00000001";
			when "0010" =>
				ld <= "00000011";
			when "0011" =>
				ld <= "00000111";
			when "0100" =>
				ld <= "00001111";
			when "0101" =>
				ld <= "00011111";
			when "0110" =>
				ld <= "00111111";
			when "0111" =>
				ld <= "01111111";
			when "1000" =>
				ld <= "11111111";
			when others =>
				null;
		end case;
		
		a_to_g <= var1;
	end if;
end process;



end Behavioral;