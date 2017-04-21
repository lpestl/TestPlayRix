--
-- ��� ����������� �������� ��������: ����� ��������, �������� � �.�.
-- ����� ����� � ��������� �������� �� �����.
--
-- ����������� �������� �������� � ����� � ������ �����������
-- �������� UploadResourceGroup, ������� ���������� ����.
--
LoadResource("Resources.xml")

--
-- �������� ��������, ��������� � ��������� ������.
--
LoadEffects("Example2_Flames.xml")
LoadEffects("Example3_FindObject.xml")
LoadEffects("fireworks.xml")

--
-- �������� ����.
--
-- ���� ��������� ��� ����� (���������� ������������ ���� �������� � �.�.),
-- �� ������ Draw() � Update() �������� ���������� ������ �����, ����� ����
-- �������� �� �����. � ������ ���������� �� ����� � �������� ���� ���������� �����
-- AcceptMessage � ���������� "Init".
--
GUI:LoadLayers("Layers.xml")

--
-- ����������� �������� ������ ��������: ��������� ������� �������, �����������
-- ����������� � ����� � �.�. ��� ���������� ��������.
--
UploadResourceGroup("TestGroup")

--
-- ���� ������� �� �����.
--
-- � ���� ������ � �������� ���� ���� ��� ���������� ����� AcceptMessage("Init")
-- � �������� ���������� ������ Draw() � Update() � ������ �����.
--
Screen:pushLayer("TestLayer")