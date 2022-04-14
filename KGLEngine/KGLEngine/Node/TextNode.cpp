// Developed by Kelin Lyu.
#include "Node.hpp"
TextNode::TextNode(Font* font, unsigned int characterCount) {
    this->font = font;
    for(unsigned int i = 0; i < characterCount; i += 1) {
        SpriteNode* characterNode = new SpriteNode(vec2(0.1f));
        this->characterNodes.push_back(characterNode);
        this->addChildNode(characterNode);
    }
}
void TextNode::setText(string text) {
    
    
    //float scale = 0.1f;
    float x = 0.0f;
    float y = 0.0f;
    
    this->scale = vec2(0.001f);
    
    unsigned int i = 0;
    string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++) {
        
        FontCharacter* character = this->font->engineGetFontCharacter(*c);
        if(character->texture == NULL) {
            continue;
        }
        if(i > this->characterNodes.size()) {
            break;
        }
        this->characterNodes[i]->isDisabled = false;
        
        
        if(character->size.x > 0) {
            this->characterNodes[i]->texture = character->texture;
        }
        
        this->characterNodes[i]->size = character->size;
        
        this->characterNodes[i]->position.x = x + character->bearing.x + character->size.x * 0.5f;
        this->characterNodes[i]->position.y = y - character->bearing.y + character->size.y * 0.5f;
        
        
        
        x += (float)(character->advance >> 6);
        
        i += 1;
    }
    for(unsigned j = i; j < this->characterNodes.size(); j += 1) {
        
        this->characterNodes[j]->isDisabled = true;
    }
}
TextNode::~TextNode() {
    for(unsigned int i = 0; i < this->characterNodes.size(); i += 1) {
        delete(this->characterNodes[i]);
    }
    this->characterNodes.clear();
    this->font = NULL;
}
void TextNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    this->UINode::enginePrepareNodeForRendering(parentWorldTransform, data);
}
void TextNode::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    this->UINode::engineCalculateNodeWorldTransform(parentWorldTransform);
}
