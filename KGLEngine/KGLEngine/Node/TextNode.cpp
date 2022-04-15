// Developed by Kelin Lyu.
#include "Node.hpp"
TextNode::TextNode(Font* font, float lineHeight, float lineLength, float lineDistance) {
    this->engineInitializeUINode();
    this->font = font;
    this->lineHeight = lineHeight;
    this->lineLength = lineLength;
    this->lineDistance = lineDistance;
    this->alignmentX = 0;
    this->alignmentY = 0;
    this->color = vec4(1.0f);
    this->text = "";
    this->currentColor = vec4(-1.0f);
    this->currentText = "";
    this->textRenderer = new TextRenderer();
    
    
    
}
TextNode::~TextNode() {
    this->font = NULL;
}
void TextNode::enginePrepareNodeForRendering(mat4 parentWorldTransform, vec2 data) {
    if(this->isDisabled) {
        return;
    }
    vec2 newData = vec2(data.x * this->alpha, data.y + this->renderingOrder);
    this->UINode::enginePrepareNodeForRendering(parentWorldTransform, data);
    
    
    
    if(this->currentText != this->text) {
        this->currentText = this->text;
        
        vector<vec2> sizes;
        vector<vec2> positions;
        vector<Texture*> textures;
        
        vector<vec2> lineSizes;
        vector<vec2> linePositions;
        vector<Texture*> lineTextures;
        
        this->alignmentX = -1;
        
        float x = 0.0f;
        float y = 0.0f;
        
        bool receivedSpace = false;
        unsigned int wordArrayIndex = 0;
        unsigned int wordStringIndex = 0;
        
        const char* characters = this->text.c_str();
        unsigned int i = 0;
        while(i < this->text.length()) {
            if(characters[i] == '\n') {
                for(unsigned int k = 0; k < lineSizes.size(); k += 1) {
                    if(this->alignmentX == 0) {
                        linePositions[k].x -= x * 0.5f;
                    }else if(this->alignmentX == 1) {
                        linePositions[k].x -= x;
                    }
                    sizes.push_back(lineSizes[k]);
                    positions.push_back(linePositions[k]);
                    textures.push_back(lineTextures[k]);
                }
                x = 0.0f;
                y += this->lineHeight;
                y += this->lineDistance;
                lineSizes.clear();
                linePositions.clear();
                lineTextures.clear();
                i += 1;
                continue;
            }
            if(characters[i] == ' ') {
                receivedSpace = true;
            }
            FontCharacter* character = this->font->engineGetFontCharacter(characters[i]);
            if(character->texture == NULL) {
                i += 1;
                continue;
            }
            if(character->size.x > 0.0f) {
                if(receivedSpace) {
                    receivedSpace = false;
                    wordArrayIndex = (unsigned int)lineSizes.size();
                    wordStringIndex = i;
                }
                lineSizes.push_back(character->size * this->lineHeight);
                vec2 newPosition;
                newPosition.x = x + character->bearing.x * this->lineHeight + character->size.x * this->lineHeight * 0.5f;
                newPosition.y = y - character->bearing.y * this->lineHeight + character->size.y * this->lineHeight * 0.5f;
                linePositions.push_back(newPosition);
                lineTextures.push_back(character->texture);
            }
            x += character->advance * this->lineHeight;
            
            
            
            if(x > this->lineLength) {
                if(wordArrayIndex == 0) {
                    wordArrayIndex = (unsigned int)lineSizes.size() - 1;
                    wordStringIndex = i;
                }
                for(unsigned int k = 0; k < wordArrayIndex; k += 1) {
                    if(this->alignmentX == 0) {
                        linePositions[k].x -= x * 0.5f;
                    }else if(this->alignmentX == 1) {
                        linePositions[k].x -= x;
                    }
                    sizes.push_back(lineSizes[k]);
                    positions.push_back(linePositions[k]);
                    textures.push_back(lineTextures[k]);
                }
                x = 0.0f;
                y += this->lineHeight;
                y += this->lineDistance;
                lineSizes.clear();
                linePositions.clear();
                lineTextures.clear();
                i = wordStringIndex - 1;
                wordArrayIndex = 0;
                wordStringIndex = 0;
            }
            
            
            
            i += 1;
        }
        for(unsigned int k = 0; k < lineSizes.size(); k += 1) {
            if(this->alignmentX == 0) {
                linePositions[k].x -= x * 0.5f;
            }else if(this->alignmentX == 1) {
                linePositions[k].x -= x;
            }
            sizes.push_back(lineSizes[k]);
            positions.push_back(linePositions[k]);
            textures.push_back(lineTextures[k]);
        }
        lineSizes.clear();
        linePositions.clear();
        lineTextures.clear();
        if(this->alignmentY == -1) {
            for(unsigned int k = 0; k < sizes.size(); k += 1) {
                positions[k].y -= y;
            }
        }else if(this->alignmentY == 0) {
            for(unsigned int k = 0; k < sizes.size(); k += 1) {
                positions[k].y -= y * 0.5f;
            }
        }
        if(sizes.size() > 0) {
            vec2 resolution = Engine::main->getScreenResolution();
            float minLength = glm::min(resolution.x, resolution.y);
            vector<mat4> transforms;
            for(unsigned int i = 0; i < sizes.size(); i += 1) {
                vec2 uiSize = sizes[i] * minLength;
                vec2 uiPosition = positions[i] * minLength;
                mat4 transform = mat4(1.0f);
                transform = glm::translate(transform, vec3(uiPosition - uiSize * 0.5f, 0.0f));
                transform = glm::scale(transform, vec3(uiSize, 1.0f));
                transforms.push_back(transform);
            }
            this->textRenderer->textures = textures;
            this->textRenderer->transforms = transforms;
            transforms.clear();
        }else{
            this->textRenderer->textures.clear();
            this->textRenderer->transforms.clear();
        }
        sizes.clear();
        positions.clear();
        textures.clear();
    }
    if(this->currentColor != this->color) {
        this->currentColor = this->color;
        
        
        
        
    }
    
    this->textRenderer->textNodeTransform = this->worldTransform;
    
    
    this->textRenderer->renderingOrder = data.y + this->renderingOrder;
    this->textRenderer->enginePrepareGeometryForRendering(mat4(0.0f));
}
void TextNode::engineCalculateNodeWorldTransform(mat4 parentWorldTransform) {
    this->UINode::engineCalculateNodeWorldTransform(parentWorldTransform);
}
