
class RelaxField:
    def __init__(self, name: str):
        self.name = name

    def __repr__(self):
        return self.name  # Ensuring meaningful representation


class RelaxClass:
    def __init__(self, name, fields: list[RelaxField]):
        self.name = name
        self.fields = fields

    def to_string(self):
        field_names = ", ".join(field.name for field in self.fields)
        return f"Class name: {self.name}, Class fields: [{field_names}]"
    
class RelaxReferenceField(RelaxField):

    def __init__(self, name: str, reference_class: RelaxClass):
        super().__init__(name)
        self.reference_class = reference_class
    
"""class RelaxObject:

    def __init__(self, clazz: 'RelaxClass', name: str, field_values: list[RelaxField]):
        self.clazz = clazz
        self.name = name
        self.field_values = field_values

    def get_field(self, field_name: str):
        for field in self.field_values:
            if field.name == field_name:
                if isinstance(field, RelaxReferenceField):
                    for object in relaxObjects:
                        if object.field_values[0] == self.field_values[0]:
                            return object
                return field.value
        raise ValueError(f"Field '{field_name}' not found")"""
    
relaxClasses = []
#relaxObjects = []

def get_class(class_name: str):
    for clazz in relaxClasses:
        if clazz.name == class_name:
            return clazz
    return None

def class_has_field(clazz: RelaxClass, field_name: str):
    for field in clazz.fields:
        if field.name == field_name:
            return True
    return False

def create_class(parts: list[str]):
    name = parts[1]
    fields = []
    for part in parts[2:]:
        if part.startswith("@"):
            referenceClass: RelaxClass = get_class(part.split(".")[0].removeprefix("@"))
            print("Checking reference class: ", part.split(".")[0].removeprefix("@"), " field: ", part.split(".")[1])
            if referenceClass is None:
                print("Reference class doesnt exist")
            elif not class_has_field(referenceClass, part.split(".")[1]):
                print("Reference field doesnt exist")
            else:
                fields.append(RelaxReferenceField(part, referenceClass))
        else:
            fields.append(RelaxField(part))
    clazz = RelaxClass(name, fields)
    relaxClasses.append(clazz)

"""def get_object(clazz: RelaxClass, name: str):
    for object in relaxObjects:
        if object.name == name:
            return object
    raise LookupError(f"Object with name: {name} not found")

def create_object(clazz: RelaxClass, name: str, field_values: list):
    field_values = list[RelaxField]
    object = RelaxObject(clazz, name, )"""

def interpret(code):
    parts = code.split()
    
    if parts[0] == "class":
        create_class(parts)

interpret("class Engine id brand weight")
interpret("class Car id name color @Engine.id")
for clazz in relaxClasses:
    print(clazz.to_string())

    