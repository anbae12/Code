
--------------------------------------------------------
--
--	This component acts as a SPI slave according to the
-- SPI TI Synchronous Serial Frame Format. 
--
--------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity SPI is
	 Generic ( 	Width 				: positive := 16);										-- The width of the data frame 
	 Port ( Clk 	  					: in  STD_LOGIC;										-- FPGAens Clk
			  -- SPI --
			  SCLK 	  					: in  STD_LOGIC;										-- Masters Clk
			  SS		  					: in  STD_LOGIC;										-- Slave Select
           MOSI 	 					: in  STD_LOGIC;										-- Master Out Slave In	  
			  MISO 	  					: out STD_LOGIC;										-- Master In Slave Out
			  Data_out_from_FPGA	  	: in  STD_LOGIC_VECTOR (15 downto 0);			-- Data from register
			  Data_into_FPGA  		: out STD_LOGIC_VECTOR (15 downto 0));			-- Data to register
end SPI;

architecture Behavioral of SPI is

	-- Signals --
	signal shift_Data_into_FPGA 			: STD_LOGIC_VECTOR (15 downto 0);
	signal shift_Data_out_from_FPGA	 	: STD_LOGIC_VECTOR (15 downto 0);
	signal SS_delayed					 	 	: STD_LOGIC_VECTOR (1 downto 0):= "00";
	signal SS_falling_edge					: STD_LOGIC := '0';
	signal SS_high_SCLK_falling			: STD_LOGIC := '0';
	signal checked								: STD_LOGIC := '0';
	signal data_done							: STD_LOGIC := '0';
	signal count								: INTEGER RANGE 0 to 15:=0;

begin

	-- Triggered on SCLK --
	Process (Clk)
	begin
	
	if rising_edge(Clk) then
		SS_delayed <= SS_delayed(0) & SS;
	
		if SS_delayed = "10" then
			SS_falling_edge <= '1';
		else 
			if checked = '1' then
				SS_falling_edge <= '0';
			end if;
		end if;
		
		if data_done = '1' then
			Data_into_FPGA <= shift_Data_into_FPGA;
		end if;
		
	end if;
	end process;


	-- Triggered on SCLK --
	Process (SCLK,SS,SS_falling_edge)
	begin
	
		-- MOSI shift register --
		if falling_edge (SCLK) then
		
		-- SS_high_SCLK_falling when sending --	
			if SS = '1' then
				SS_high_SCLK_falling <= '1';
			else 
				SS_high_SCLK_falling <= '0';
			end if;
		---------------------------------------
		
			shift_Data_into_FPGA(15 downto 1) <= shift_Data_into_FPGA(14 downto 0);
			shift_Data_into_FPGA(0) <= MOSI;
	
			if SS_falling_edge = '1' then
				count <= 0;
				checked <= '1';
			else 
				if count = 14 then
					checked <= '0';
					data_done <= '1';
					count <= count + 1;
				else
					count <= count + 1;
					checked <= '0';
					data_done <= '0';
				end if;
			end if;
		end if;
		
		-- MISO shift register -- 
		if rising_edge (SCLK) then
			if SS_high_SCLK_falling = '1' then 
				shift_Data_out_from_FPGA <= Data_out_from_FPGA;
			else
				shift_Data_out_from_FPGA (15 downto 1) <= shift_Data_out_from_FPGA	 (14 downto 0);
				shift_Data_out_from_FPGA	 (0) <= '0';
			end if;
		end if;			
		
	end process;

	
	MISO <= shift_Data_out_from_FPGA	(15) ;
	
	
end Behavioral;

