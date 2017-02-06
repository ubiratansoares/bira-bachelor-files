APP_NAME = agenda
APP_PATH = $(APPNAME)

SOURCE_DIR = src
STATIC_DIR = web
CONFIG_DIR = cfg
BUILD_DIR = bld
LIBS_DIR = lib
DIST_DIR = dst

SOURCE_FILES = \
$(SOURCE_DIR)/tpw/agenda/database/ConnectionManager.java \
$(SOURCE_DIR)/tpw/agenda/database/DatabaseObject.java \
$(SOURCE_DIR)/tpw/agenda/database/DatabaseException.java \
$(SOURCE_DIR)/tpw/agenda/database/DatabaseUtilities.java \
$(SOURCE_DIR)/tpw/agenda/model/Account.java \
$(SOURCE_DIR)/tpw/agenda/model/Privileges.java

CLASS_FILES = \
$(BUILD_DIR)/tpw/agenda/database/ConnectionManager.class \
$(BUILD_DIR)/tpw/agenda/database/DatabaseObject.class \
$(BUILD_DIR)/tpw/agenda/database/DatabaseException.class \
$(BUILD_DIR)/tpw/agenda/database/DatabaseUtilities.class \
$(BUILD_DIR)/tpw/agenda/model/Account.class \
$(BUILD_DIR)/tpw/agenda/model/Privileges.class

.PHONY = clean build dist run

$(SOURCE_DIR): 
	mkdir $(SOURCE_DIR)

$(STATIC_DIR):
	mkdir $(STATIC_DIR)

$(CONFIG_DIR):
	mkdir $(CONFIG_DIR)

$(LIBS_DIR):
	mkdir $(LIBS_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(DIST_DIR):
	mkdir $(DIST_DIR)
	mkdir $(DIST_DIR)/WEB-INF
	mkdir $(DIST_DIR)/WEB-INF/classes
	mkdir $(DIST_DIR)/WEB-INF/lib
	mkdir $(DIST_DIR)/META-INF
	
	cp -R $(BUILD_DIR) $(DIST_DIR)/WEB_INF/classes

$(CLASS_FILES): $(SOURCE_DIR) $(BUILD_DIR) $(LIBS_DIR) $(SOURCE_FILES)
	javac -sourcepath $(SOURCE_DIR) -cp $(BUILD_DIR):$(LIBS_DIR)/* -d $(BUILD_DIR) -g $(SOURCE_FILES)

clean:
	rm -r $(BUILD_DIR)
	rm -r $(DIST_DIR)
	rm $(APP_NAME).war

build: $(CLASS_FILES)

dist: $(APP_NAME).war

run: $(BUILD_DIR)/Main.class
	java -cp $(BUILD_DIR):$(LIBS) Main
