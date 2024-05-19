# Running this macro will create a new FreeCAD document containing the
# geometry.

import FreeCAD

def mkVec(**kwargs):
  return FreeCAD.Vector(kwargs["x"], kwargs["y"], kwargs["z"])

class Size:
  def __init__(self, **kwargs):
    self.length = kwargs["length"]
    self.width = kwargs["width"]
    self.height = kwargs["height"]

  def vec(self):
    return mkVec(x = self.length, y = self.width, z = self.height)

def mkSize(**kwargs):
  return Size(**kwargs)

class Doc:
  def __init__(self, doc):
    self.doc = doc

  @staticmethod
  def new():
    return Doc(FreeCAD.newDocument())

  def recompute(self):
    self.doc.recompute()

  def box(self, **kwargs):
    box = Box(self.doc)
    box.setPlacement(kwargs["position"])
    box.setSize(kwargs["size"])
    return box

  def cut(self, **kwargs):
    cut = Cut(self.doc)
    cut.setBase(kwargs["base"])
    cut.setTool(kwargs["tool"])
    self.recompute()
    return cut

  def fillet(self, **kwargs):
    fillet = Fillet(self.doc)
    fillet.setBase(kwargs["base"])
    fillet.setEdges(kwargs["edges"])
    kwargs["base"].obj.Visibility = False
    self.recompute()
    return fillet

  def case_(self, **kwargs):
    return Case(self, **kwargs)

class Box:
  def __init__(self, doc):
    self.obj = doc.addObject("Part::Box")

  def setPlacement(self, vec):
    self.obj.Placement.Base = vec

  def setSize(self, size):
    self.obj.Length = size.length
    self.obj.Width = size.width
    self.obj.Height = size.height

class Cut:
  def __init__(self, doc):
    self.obj = doc.addObject("Part::Cut")

  def setBase(self, base):
    self.obj.Base = base.obj

  def setTool(self, tool):
    self.obj.Tool = tool.obj

class Fillet:
  def __init__(self, doc):
    self.obj = doc.addObject("Part::Fillet")
    self.obj.Edges = []

  def setBase(self, base):
    self.obj.Base = base.obj

  def setEdges(self, edges):
    filletEdges = []
    for (edgesIndex, radiusDegrees) in edges:
      filletEdges.append((edgesIndex, radiusDegrees, radiusDegrees))
    self.obj.Edges = filletEdges

class Case:
  def __init__(self, doc, **kwargs):
    position = kwargs["position"]
    innerSize = kwargs["innerSize"]
    thickness = kwargs["thickness"]
    innerBox = doc.box(
      position = position,
      size = innerSize,
    )
    innerBoxFilleted = doc.fillet(
      base = innerBox,
      edges = [(1, 25), (5, 25), (3, 5), (7, 5)],
    )
    outerBox = doc.box(
      position = position - mkVec(x = thickness, y = thickness, z = thickness),
      size = mkSize(
        length = innerSize.length + (thickness  * 2),
        width = innerSize.width + (thickness  * 2),
        height = innerSize.height + thickness,
      ),
    )
    outerBoxFilleted = doc.fillet(
      base = outerBox,
      edges = [(1, 30), (5, 30), (3, 10), (7, 10)],
    )
    doc.cut(
      base = outerBoxFilleted,
      tool = innerBoxFilleted,
    )

doc = Doc.new()
case_ = doc.case_(
  position = mkVec(x = 0, y = 0, z = 0),
  innerSize = mkSize(length = 437, width = 313, height = 84),
  thickness = 9,
)
print("xxx")
