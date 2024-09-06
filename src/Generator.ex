# Module definition
defmodule Generator do
	# content
	defstruct content: %{}

	# Creating the content
	def parse() do
		{:ok, content} = File.read "shortcut"
		content = String.split content, "\n"
		programs = String.split content[0], " "
		keys = String.split content[1], " "
	end

	# Writing to some file
	def write() do

	end	
end
