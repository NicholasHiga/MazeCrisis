#include "AbstractRenderer.h"

#include <vector>
#include "GameObject.h"
#include "Renderable.h"
#include "SceneGraph.h"
#include "SceneNode.h"
#include "Camera.h"

using std::vector;
using std::shared_ptr;

SceneGraph::SceneGraph()
{
    root = std::make_shared<SceneNode>();
}

shared_ptr<SceneNode>
SceneGraph::getRootSceneNode() const
{
    return root;
}

void
SceneGraph::doSomethingEachElement(shared_ptr<SceneNode> firstNode, 
    std::function<void(shared_ptr<SceneNode>)> func)
{
    for (size_t i = 0; i < firstNode.get()->getChildren()->size(); ++i)
    {
        doSomethingEachElement((*firstNode.get()->getChildren())[i], func);
        func((*firstNode.get()->getChildren())[i]);
    }
}

void 
SceneGraph::renderSceneGraph(AbstractRenderer &renderer, Camera &camera)
{
    // prepareToRender() if rendeRootSceneNode does not call it
    // before hand.
    renderer.renderSceneGraph(*this, camera);
}

void
SceneGraph::updateSceneGraph(double deltaTime)
{
    doSomethingEachElement(root,
        [deltaTime](shared_ptr<SceneNode> n)
    {
        vector<Renderable*> *tmp = n.get()->getRenderables();
        for (size_t i = 0; i < (*tmp).size(); ++i)
        {
            GameObject *t = dynamic_cast<GameObject*>((*tmp)[i]);
            if (t)
                t->update(deltaTime);
        }
    });
}

void 
SceneGraph::appendChildNode(shared_ptr<SceneNode> node)
{
    root->appendChildNode(node);
}

bool
SceneGraph::removeChildNode(shared_ptr<SceneNode> node)
{
    bool childRemoved = false;
    // Inefficient, because unable to break from the lambda
    // immediately after the renderable is found.
    doSomethingEachElement(root,
        [&childRemoved, node](shared_ptr<SceneNode> n)
    {
        if (!childRemoved && n.get() == node.get())
        {
            n.get()->getParentNode()->removeChildNode(n);
            childRemoved = true;
        }
    });
    return childRemoved;
}

bool
SceneGraph::removeChildNode(SceneNode *node)
{
    bool childRemoved = false;
    // Inefficient, because unable to break from the lambda
    // immediately after the renderable is found.
    doSomethingEachElement(root,
        [&childRemoved, node](shared_ptr<SceneNode> n)
    {
        if (!childRemoved && n.get() == node)
        {
            n.get()->getParentNode()->removeChildNode(n);
            childRemoved = true;
        }
    });
    return childRemoved;
}

bool 
SceneGraph::removeNodeWithRenderable(Renderable &renderable)
{
    bool renderableRemoved = false;
    // Inefficient, because unable to break from the lambda
    // immediately after the renderable is found.
    doSomethingEachElement(root,
        [&renderableRemoved, &renderable](shared_ptr<SceneNode> n)
    {
        if (!renderableRemoved)
        {
            vector<Renderable*> *tmp = n.get()->getRenderables();
            for (size_t i = 0; i < (*tmp).size(); ++i)
            {
                if (&renderable == (*tmp)[i])
                {
                    n.get()->removeRenderable(renderable);
                    n.get()->getParentNode()->removeChildNode(n);
                    renderableRemoved = true;
                }
            }
        }
    });
    return renderableRemoved;
}

void
SceneGraph::removeAllNodes()
{
    root = nullptr;
    root = std::make_shared<SceneNode>();
}