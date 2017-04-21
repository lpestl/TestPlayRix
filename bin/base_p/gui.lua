function TestLayerFunc(message)
	WriteMessage(message:getPublisher().." - "..message:getData())
end