import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SpaceAddComponent } from './component';

describe('SpaceAddComponent', () => {
  let component: SpaceAddComponent;
  let fixture: ComponentFixture<SpaceAddComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
    imports: [SpaceAddComponent]
})
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SpaceAddComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
